<style>
    pre {outline: 1px solid #ccc; padding: 5px; margin: 5px; }
    .string { color: green; }
    .number { color: darkorange; }
    .boolean { color: blue; }
    .null { color: magenta; }
    .key { color: red; }
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<form id="ws-form" method="post" action="/webservice/wsRequisicao.php">

    Função: <input type="text" name="funcao" value=""/>
    Usuário: <input type="text" name="usuario" value=""/>
    Senha: <input type="password" name="senha" value=""/>
    Dados: <input type="text" name="dados" value=""/>
    Módulo: <input type="text" name="modulo" value=""/>

    <input type="submit" value="Enviar"/>
</form>
<pre id="result">

</pre>

<script type="text/javascript">

    function output(inp) {
        $('#result').html(inp);
    }

    function syntaxHighlight(json) {
        json = json.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
        return json.replace(/("(\\u[a-zA-Z0-9]{4}|\\[^u]|[^\\"])*"(\s*:)?|\b(true|false|null)\b|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?)/g, function (match) {
            var cls = 'number';
            if (/^"/.test(match)) {
                if (/:$/.test(match)) {
                    cls = 'key';
                } else {
                    cls = 'string';
                }
            } else if (/true|false/.test(match)) {
                cls = 'boolean';
            } else if (/null/.test(match)) {
                cls = 'null';
            }
            return '<span class="' + cls + '">' + match + '</span>';
        });
    }

    $(function(){
        $('#ws-form').submit(function(e){
            e.preventDefault();

            $.ajax({
                type: "POST",
                url: '/webservice/wsRequisicao.php',
                data: $("#ws-form").serialize(), // serializes the form's elements.
                beforeSend: function(data){
                    output('Carregando...');
                },
                success: function(data)
                {
                    data = eval("(" + data + ')');
                    data = JSON.stringify(data, undefined, 4)
                    output(syntaxHighlight(data));
                }
            });
        });
    });
</script>