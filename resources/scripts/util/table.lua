local function tableMerge(t1, t2)
    for k, v in pairs(t2) do
        if type(t1[k]) == 'table' and
                type(v) == 'table' then
            tableMerge(t1[k], v)
        else
            t1[k] = v
        end
    end
end

return {
    tableMerge = tableMerge,
}
