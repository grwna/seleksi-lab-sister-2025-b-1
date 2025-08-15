const protectPage = () => {
    const token = localStorage.getItem('jwtToken');
    if (!token) {
        if (window.location.pathname !== '/' && window.location.pathname !== '/index.html') {
            window.location.href = '/';
        }
    }
};

const handleLogout = () => {
    localStorage.removeItem('jwtToken');
    window.location.href = '/';
};

const initIndexPage = () => {
    const mainActionBtn = document.getElementById('main-action-btn');

    const urlParams = new URLSearchParams(window.location.search);
    const tokenFromUrl = urlParams.get('token');

    if (tokenFromUrl) {
        localStorage.setItem('jwtToken', tokenFromUrl);
        window.location.href = '/';
        return;
    }

    const token = localStorage.getItem('jwtToken');

    if (token) {
        mainActionBtn.textContent = 'Get Started';
        mainActionBtn.href = '/summary.html';
    } else {
        mainActionBtn.textContent = 'Login';
        mainActionBtn.href = '/auth/google';
    }
};

const initSummaryPage = () => {
    protectPage();
    const summaryForm = document.getElementById('summary-form');
    const sourceText = document.getElementById('source-text');
    const summaryOutput = document.getElementById('summary-output');
    const logoutBtn = document.getElementById('logout-btn');

    logoutBtn.addEventListener('click', handleLogout);

    summaryForm.addEventListener('submit', async (event) => {
        event.preventDefault();
        summaryOutput.textContent = 'Memproses...';
        const token = localStorage.getItem('jwtToken');

        try {
            const response = await fetch('/api/summarize', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': `Bearer ${token}`
                },
                body: JSON.stringify({ text: sourceText.value })
            });

            if (!response.ok) {
                throw new Error('Gagal membuat ringkasan. Silakan coba lagi.');
            }

            const data = await response.json();
            summaryOutput.textContent = data.summary;
        } catch (error) {
            summaryOutput.textContent = `Error: ${error.message}`;
        }
    });
};

const initHistoryPage = () => {
    protectPage();
    const historyList = document.getElementById('history-list');
    const logoutBtn = document.getElementById('logout-btn');
    const token = localStorage.getItem('jwtToken');

    // Elemen Modal
    const modal = document.getElementById('history-modal');
    const closeModalBtn = document.getElementById('close-modal-btn');
    const modalSourceText = document.getElementById('modal-source-text');
    const modalSummaryText = document.getElementById('modal-summary-text');
    const modalDeleteBtn = document.getElementById('modal-delete-btn');

    logoutBtn.addEventListener('click', handleLogout);

    const fetchHistory = async () => {
        historyList.innerHTML = '<li>Memuat riwayat...</li>';
        try {
            const response = await fetch('/api/history', {
                headers: { 'Authorization': `Bearer ${token}` }
            });
            if (!response.ok) { throw new Error('Gagal memuat riwayat.'); }
            
            const histories = await response.json();
            historyList.innerHTML = '';

            if (histories.length === 0) {
                historyList.innerHTML = '<li>Anda belum memiliki riwayat.</li>';
                return;
            }

            histories.forEach(item => {
                const li = document.createElement('li');
                li.dataset.id = item.summary_id;
                li.innerHTML = `
                    <p>${item.summarized_text.substring(0, 150)}...</p>
                    <small>${new Date(item.created_at).toLocaleString()}</small>
                `;
                historyList.appendChild(li);
            });

        } catch (error) {
            historyList.innerHTML = `<li>Error: ${error.message}</li>`;
        }
    };

    const showDetails = async (summaryId) => {
        try {
            const response = await fetch(`/api/history/${summaryId}`, {
                headers: { 'Authorization': `Bearer ${token}` }
            });
            if (!response.ok) { throw new Error('Gagal mengambil detail.'); }
            
            const data = await response.json();
            modalSourceText.textContent = data.source_text;
            modalSummaryText.textContent = data.summarized_text;
            modalDeleteBtn.dataset.id = summaryId;
            modal.classList.remove('hidden');
        } catch (error) {
            alert(`Error: ${error.message}`);
        }
    };

    const handleDelete = async () => {
        const summaryId = modalDeleteBtn.dataset.id;
        if (confirm('Apakah Anda yakin ingin menghapus entri ini?')) {
            try {
                const response = await fetch(`/api/history/${summaryId}`, {
                    method: 'DELETE',
                    headers: { 'Authorization': `Bearer ${token}` }
                });
                if (!response.ok) { throw new Error('Gagal menghapus riwayat.'); }
                
                modal.classList.add('hidden');
                fetchHistory();
            } catch (error) {
                alert(`Error: ${error.message}`);
            }
        }
    };

    historyList.addEventListener('click', (event) => {
        const listItem = event.target.closest('li');
        if (listItem && listItem.dataset.id) {
            showDetails(listItem.dataset.id);
        }
    });
    
    closeModalBtn.addEventListener('click', () => modal.classList.add('hidden'));
    modalDeleteBtn.addEventListener('click', handleDelete);

    fetchHistory();
};

document.addEventListener('DOMContentLoaded', () => {
    const path = window.location.pathname;

    if (path === '/' || path === '/index.html') {
        initIndexPage();
    } else if (path === '/summary.html') {
        initSummaryPage();
    } else if (path === '/history.html') {
        initHistoryPage();
    }
});
