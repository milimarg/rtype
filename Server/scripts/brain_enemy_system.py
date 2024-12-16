def run(v):
    if ("ascending" not in v) or ("x" not in v) or ("y" not in v):
        return
    if v["ascending"]:
        if v["y"] <= 200:
            v["ascending"] = False
        v["y"] -= 5
    else:
        if v["y"] >= 800:
            v["ascending"] = True
        v["y"] += 5
    v["x"] -= 1
    return v
