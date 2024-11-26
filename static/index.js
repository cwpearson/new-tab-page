function updateClock() {
    console.log("updateClock()...")
    const now = new Date();
    const timeString = now.toLocaleTimeString('en-US', { 
        hour12: false,
        hour: '2-digit',
        minute: '2-digit',
        second: '2-digit'
    });
    const dateString = now.toLocaleDateString('en-US', { 
        weekday: 'long',
        month: 'long',
        day: 'numeric'
    });
    
    document.getElementById('clock').textContent = timeString;
    document.getElementById('date').textContent = dateString;
}

// Update clock immediately and then every second
updateClock();
setInterval(updateClock, 1000);

function updateProgressDay() {
    const now = new Date();
    const start = new Date();
    start.setHours(9, 0, 0, 0);
    const end = new Date();
    end.setHours(17, 0, 0, 0);

    const progressBar = document.getElementById('progress-day');
    
    // Calculate progress
    let progress = 0;
    if (now < start) {
      progress = 0;
    } else if (now > end) {
      progress = 100;
      progressBar.classList.add('complete');
    } else {
      const total = end - start;
      const current = now - start;
      progress = (current / total) * 100;
    }

    progressBar.style.width = progress + '%';
  }

  // Update immediately and then every second
  updateProgressDay();
  setInterval(updateProgressDay, 1000);


// Kagi search
const kagiBar = document.getElementById('kagi-search');
kagiBar.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') {
        const query = kagiBar.value.trim();
        if (query) {
            window.location.href = `https://kagi.com/search?q=${encodeURIComponent(query)}`;
        }
    }
});

// Handle search
const claudeBar = document.getElementById('claude-search');
claudeBar.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') {
        const query = claudeBar.value.trim();
        if (query) {
            window.location.href = `https://claude.ai/new?q=${encodeURIComponent(query)}`;
        }
    }
});

function getYearStart() {
    const now = new Date();
    const currentYear = now.getFullYear();
    // If we're before Oct 21, use previous year's Oct 21
    if (now.getMonth() < 9 || (now.getMonth() === 9 && now.getDate() < 21)) {
        return new Date(currentYear - 1, 9, 21);
    }
    return new Date(currentYear, 9, 21);
}

function getYearEnd() {
    const start = getYearStart();
    return new Date(start.getFullYear() + 1, 9, 21);
}

  function updateProgressFy() {
    const now = new Date();
    const start = getYearStart();
    const end = getYearEnd();

    const progressBar = document.getElementById('progress-fy');

    // Calculate progress
    const total = end - start;
    const current = now - start;
    const progress = (current / total) * 100;
    
    progressBar.style.width = progress.toFixed(2) + '%';

    // Calculate days elapsed and remaining
    const daysElapsed = Math.floor(current / (1000 * 60 * 60 * 24));
    const daysRemaining = Math.ceil((end - now) / (1000 * 60 * 60 * 24));
  }

  // Update immediately and then every minute
  updateProgressFy();
  setInterval(updateProgressFy, 60000);