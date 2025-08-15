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
    const logoutBtn = document.getElementById('logout-btn');
    logoutBtn.addEventListener('click', handleLogout);
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
