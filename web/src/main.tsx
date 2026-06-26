import React from "react";
import ReactDOM from "react-dom/client";
import { HashRouter, Routes, Route } from "react-router-dom";
import Home from "./pages/Home";
import Aventurero from "./pages/Aventurero";
import Manual from "./pages/Manual";
import "./styles.css";

ReactDOM.createRoot(document.getElementById("root")!).render(
    <React.StrictMode>
        <HashRouter>
            <Routes>
                <Route path="/" element={<Home />} />
                <Route path="/aventurero" element={<Aventurero />} />
                <Route path="/manual" element={<Manual />} />
            </Routes>
        </HashRouter>
    </React.StrictMode>,
);