<?php
    if ($_REQUEST['carrega'] && $_REQUEST['id']) {
        carregaDados($_REQUEST['id']);
        exit;
    }

    if ($_POST["submeter"] == 'excluir' && $_POST["preid"]) {
        $existe_ur = $db->pegaUm("SELECT ureid FROM evento.unidaderecurso_old WHERE preid='" . $_POST["preid"] . "'");
        if ($existe_ur){
            die("<script>alert('Pregão não pode ser excluído. Existe unidade de recurso vinculada ao pregão');window.location='evento.php?modulo=principal/cadPregao&acao=A';</script>");
        }
        $sql = "DELETE FROM evento.pregaoevento_old WHERE preid = " . $_POST["preid"];
        $db->executar($sql);
        
        if ($db->commit()) {
            $db->sucesso('principal/cadPregao', '&id=' . $_POST['preid']);
        } else {
            $db->insucesso('Operação não realizada com sucesso', '&id=' . $_POST['preid'], 'principal/cadPregao');
        }
    }

    if ($_POST["submeter"] == 'salvar') {
        $dataIni        = formata_data_sql($_POST['preiniciovig']);
        $dataFim        = formata_data_sql($_POST['prefimvig']);
        $vrCont         = str_replace('.', '', $_POST['prevalorcontratado']);
        $vrCont         = str_replace(',', '.', $vrCont);
        $vrEmp          = str_replace('.', '', $_POST['prevalorempenhado']);
        $vrEmp          = str_replace(',', '.', $vrEmp);
        $vrRec          = str_replace('.', '', $_POST['limite']);
        $vrRec          = str_replace(',', '.', $vrRec);
        $numproc        = str_replace(array('.', '/', '-'), '', $_POST['prenumprocesso']);
        $cnpj           = str_replace(array('.', '/', '-'), '', $_POST['precnpj']);
        $razao          = trim( $_POST['prerazaosocial'] );
        $prenumcontrato = trim( $_POST['prenumcontrato'] );

        if (!$_POST["preid"] || $_POST["preid"] == ""){
            $sql = "
                INSERT INTO evento.pregaoevento_old(
                        precodpregao, 
                        predescpregao, 
                        preiniciovig, 
                        prefimvig, 
                        prevalorcontratado, 
                        prevalorempenhado,			          
                        prenumprocesso,
                        precnpj,
                        prerazaosocial,
                        prenumcontrato
                    )VALUES(
                        '" . $_POST['precodpregao'] . "',
                        '" . substr($_POST['predescpregao'], 0, 1000) . "', 
                        '" . $dataIni . "', 
                        '" . $dataFim . "', 
                        '" . $vrCont . "', 
                        '" . $vrEmp . "',
                        '" . $numproc . "',
                        '" . $cnpj . "',
                        '" . $razao . "',
                        '" . $prenumcontrato . "'  
                   ) RETURNING preid
            ";
            $db->executar($sql);
        } else {
            $sql = "
                UPDATE evento.pregaoevento_old
                        SET precodpregao        = '" . $_POST['precodpregao'] . "', 
                            predescpregao       = '" . $_POST['predescpregao'] . "',
                            preiniciovig        = '" . $dataIni . "',
                            prefimvig           = '" . $dataFim . "', 
                            prevalorcontratado  = '" . $vrCont . "', 
                            prevalorempenhado   = '" . $vrEmp . "',			          
                            prenumprocesso      = '" . $numproc . "',
                            precnpj             = '" . $cnpj . "',
                            prerazaosocial      = '" . $razao . "',
                            prenumcontrato      = '" . $prenumcontrato . "'
                        WHERE preid = " . $_POST['preid'];
                $db->executar($sql);
        }
        
        if ($db->commit()) {
            $db->sucesso('principal/cadPregao');
        } else {
            $db->insucesso('Operação não realizada com sucesso', '', 'principal/cadPregao');
        }
    }
    
?>
    <head>
        <meta http-equiv="content-language" content="pt-br">
        <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
        
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script src="../includes/prototype.js"></script>
        <script src="../includes/entidades.js"></script>
        <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
        <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    </head>

<?php
        $titulo_modulo = "Ata/Contrato";
        //Chamada de programa
        include APPRAIZ . "includes/cabecalho.inc";
        echo'<br>';
        monta_titulo($titulo_modulo, 'Cadastro das Atas/Contratos');
        echo'<br>';

        $prenumprocesso     = $DadosPregao[0]["prenumprocesso"];
        $precodpregao       = $DadosPregao[0]["precodpregao"];
        $prenumcontrato     = $DadosPregao[0]["prenumcontrato"];
        $predescpregao      = $DadosPregao[0]["predescpregao"];
        $preiniciovig       = $DadosPregao[0]["preiniciovig"];
        $prefimvig          = $DadosPregao[0]["prefimvig"];
        $prevalorcontratado = $DadosPregao[0]["prevalorcontratado"];
        $prevalorempenhado  = $DadosPregao[0]["prevalorempenhado"];
        $prenumcontrato     = $DadosPregao[0]["prenumcontrato"];
?>	
    
    <form name ="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
        <input type="hidden" name="submeter" id="submeter" value="">
        <input type="hidden" name="preid" id="preid" value="">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class ="SubTituloDireita" align="right">Número Processo: 
                    <input type="hidden" name="numero" id="numero" value="0" />
                </td>
                <td> 
                    <?php
                        $prenumprocesso = $DadosPregao[0]['prenumprocesso']; 
                        echo campo_texto('prenumprocesso', 'S', $somenteLeitura, '', 24, 20, '', '', 'left', '', 0, 'id="prenumprocesso" onblur="MouseBlur(this);"'); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Código: 
                </td>
                <td> 
                    <?php
                        $precodpregao = $DadosPregao[0]['precodpregao']; 
                        echo campo_texto('precodpregao', 'S', $somenteLeitura, '', 24, 7, '', '', 'left', '', 0, 'id="precodpregao" onblur="MouseBlur(this);"'); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Número do Contrato: 
                </td>
                <td> 
                    <?php
                        $prenumcontrato = $DadosPregao[0]['prenumcontrato']; 
                        echo campo_texto('prenumcontrato', 'S', $somenteLeitura, '', 24, 20, '', '', 'left', '', 0, 'id="prenumcontrato" onblur="MouseBlur(this);"'); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Descrição:</td>
                <td> 
                    <?php
                        $predescpregao = $DadosPregao[0]['predescpregao']; 
                        echo campo_textarea('predescpregao', 'S', 'S', '', 50, 3, '1000', '', '0', '', '', '', $predescpregao ); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Inicio da Vigência:</td>
                <td>
                    <?php
                        $preiniciovig = $DadosPregao[0]["preiniciovig"];
                        echo campo_data2('preiniciovig', 'S', 'S', 'Inicio da Vigência', '##/##/####'); 
                    ?> 
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Fim da Vigência:</td>
                <td>
                    <?php 
                        $prefimvig = $DadosPregao[0]["prefimvig"];
                        echo campo_data2('prefimvig', 'S', 'S', 'Fim da Vigência', '##/##/####');
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Valor Contratado: </td>
                <td> 
                    <?php
                        $prevalorcontratado = number_format($DadosPregao[0]["prevalorcontratado"], 2, ",", ".");
                        echo campo_texto('prevalorcontratado', 'S', $somenteLeitura, '', 20, 20, '###.###.###.###,##', '', 'left', '', 0, 'id="prevalorcontratado" onblur="MouseBlur(this);"'); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Valor Empenhado: </td>
                <td> 
                    <?php
                        $prevalorempenhado = number_format($DadosPregao[0]["prevalorempenhado"], 2, ",", ".");
                        echo campo_texto('prevalorempenhado', 'S', $somenteLeitura, '', 20, 20, '###.###.###.###,##', '', 'left', '', 0, 'id="prevalorempenhado" onblur="MouseBlur(this);"'); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">CNPJ: </td>
                <td> 
                    <?php
                        $precnpj = $DadosPregao[0]["precnpj"];
                        echo campo_texto('precnpj', 'S', $somenteLeitura, '', 20, 20, '##.###.###/####-##', '', 'left', '', 0, 'id="precnpj" onblur="MouseBlur(this);"'); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Razão Social: </td>
                <td> 
                    <?php
                        $prerazaosocial = $DadosPregao[0]["prerazaosocial"];
                        echo campo_texto('prerazaosocial', 'S', $somenteLeitura, '', 50, 100, '', '', 'left', '', 0, 'id="prerazaosocial" onblur="MouseBlur(this);"'); 
                    ?>
                </td>
            </tr>
        </table>
        
        <table class="tabela" border="0" bgcolor="#dcdcdc" cellSpacing="1" cellPadding="0" align="center" border="0">
            <tr>
                <td style="text-align: center;">
                    <input type="button" value="Salvar" id="pgSalvar" name="pgSalvar" onclick="validaFormulario();"/> 
            </tr>
        </table>
    </form>

    <form name ="formularioUnidade" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formularioUnidade"> 
        <table class="listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0">

        <?php
            $sql = "
                SELECT '<center>
                            <img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('||pe.preid||')\" \" border=0 alt=\"Ir\" title=\"Alterar\"> <img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('||pe.preid||');\" border=0 alt=\"Ir\" title=\"Excluir\">
                        </center>' as acao,
                        pe.prenumprocesso || ' ' as prenumprocesso,
                        pe.precodpregao || ' ' as precodpregao,
                        'Nº: '||pe.prenumcontrato || '' as prenumcontrato, 
                        CAST(pe.predescpregao AS text)|| ' ' as predescpregao, 
                        to_char(pe.preiniciovig,'dd/mm/YYYY') as preiniciovig, 
                        to_char(pe.prefimvig,'dd/mm/YYYY') as prefimvig, 
                        pe.prevalorcontratado, 
                        pe.prevalorempenhado
                FROM evento.pregaoevento_old pe
                ORDER BY pe.preid
            ";
            $cabecalho = array("Ações", "Número do Processo", "Código do Pregão", "Número do Contrato", "Descrição do Pregão", "Início de Vigencia", "Fim de Vigencia", "Valor Contratado", "Valor Empenhado");
            $db->monta_lista_grupo($sql, $cabecalho, 25, 10, 'S', '95%', 'N', '');
        ?>
        </table>
    </form>

    <script type="text/javascript">
        function validaFormulario() {

            var prenumprocesso = document.getElementById('prenumprocesso').value;
            var precodpregao = document.getElementById('precodpregao').value;
            var predescpregao = document.getElementById('predescpregao').value;
            var datainiciovig = document.getElementById('preiniciovig');
            var datafimvig = document.getElementById('prefimvig');
            var prevalorcontratado = document.getElementById('prevalorcontratado').value;
            var prevalorempenhado = document.getElementById('prevalorempenhado').value;
            var precnpj = document.getElementById('precnpj').value;
            var prerazaosocial = document.getElementById('prerazaosocial').value;

            if (prenumprocesso == ''){
                alert('O Campo "Número do Processo" é Obrigatório');
                return false;
            }
            if (precodpregao == ''){
                alert('O Campo "Código do Pregão" é Obrigatório');
                return false;
            }
            if (predescpregao == ''){
                alert('O Campo "Descrição do Pregão" é Obrigatório');
                return false;
            }
            if (datainiciovig == ''){
                alert('O Campo "Início da Vigência do Pregão" é Obrigatório');
                return false;
            }
            if (datafimvig == ''){
                alert('O Campo "Fim da Vigência do Pregão" é Obrigatório');
                return false;
            }
            if (prevalorcontratado == ''){
                alert('O Campo "Valor Contratado do Pregão" é Obrigatório');
                return false;
            }
            if (prevalorempenhado == ''){
                alert('O Campo "Valor Empenhado do Pregão" é Obrigatório');
                return false;
            }
            if (precnpj == ''){
                alert('O Campo "CNPJ" é Obrigatório');
                return false;
            }
            if (prerazaosocial == ''){
                alert('O Campo "Razão Social" é Obrigatório');
                return false;
            }
            if (!validaDataMaior(datainiciovig, datafimvig)){
                alert("A data de 'Início da Vigência' não pode ser maior do que a data do'Fim da Vigência'.");
                datafimvig.focus();
                datafimvig.value = '';
                return false;
            }
            document.getElementById('submeter').value = 'salvar';
            document.getElementById('formulario').submit();
        }

        function salvar(){
            document.getElementById('formularioUnidade').submit();
        }

        function conferirDigitoVerificador() {
            var txtDigitado = campoNumeroProcessoSidoc;
            var atual = "";
            var conta = 14;
            var conta2 = 0;
            var numero = "";
            var valor = 0;
            var digitoVerif = "";
            var digito1 = "";
            var digito2 = "";
            var ano = "";

            txtDigitado = txtDigitado.substr(12, 17);
            digitoVerif = txtDigitado.substr(15, 2);
            numero = txtDigitado.substr(0, 15);
            conta2 = 1;
            for (conta = 14; conta >= 0; conta--) {
                var conta2 = conta2 + 1;
                var algarismo = numero.substr(conta, 1);
                valor = valor + eval(algarismo) * conta2;
            }
            if (valor % 11 == 0) {
                digito1 = "1";
            }else if (valor % 11 == 1) {
                digito1 = "0";
            } else {
                digito1 = "" + (11 - (valor % 11));
            }
            numero = numero + digito1;
            valor = 0;
            conta2 = 1;
            for (conta = 15; conta >= 0; conta--) {
                var conta2 = conta2 + 1;
                var algarismo = numero.substr(conta, 1);
                valor = valor + eval(algarismo) * conta2;
            }
            if (valor % 11 == 0) {
                digito2 = "1";
            } else if (valor % 11 == 1) {
                digito2 = "0";
            } else {
                digito2 = "" + (11 - (valor % 11));
            }
            if (numero) {
                var result = digitoVerif == (digito1 + digito2);
            } else {
                var result = true;
            }
            alert(result);
            if (result) {
                return true;
            } else {
                var mensagemDeErro = "Digito verificador não confere.";
                return false;
            }
        }

        function alterar(id){
            var myAjax = new Ajax.Request('evento.php?modulo=principal/cadPregao&acao=A', {
                method: 'POST',
                parameters: "carrega=true&id=" + id,
                onComplete: function(res) {
                    if (res.responseText) {
                        dados = res.responseText;
                        dados = dados.split('|');
                        document.getElementById('preid').value = id;
                        document.getElementById('precodpregao').value = dados[0];
                        document.getElementById('predescpregao').value = dados[1];
                        document.getElementById('preiniciovig').value = dados[2];
                        document.getElementById('prefimvig').value = dados[3];
                        document.getElementById('prevalorcontratado').value = dados[4];
                        document.getElementById('prevalorempenhado').value = dados[5];
                        document.getElementById('prenumprocesso').value = dados[6];
                        document.getElementById('precnpj').value = dados[7];
                        document.getElementById('prerazaosocial').value = dados[8];
                        document.getElementById('prenumcontrato').value = dados[9];
                    }
                }
            });
        }

        function excluir(id){
            if (confirm("Deseja excluir este item?")){
                var formulario = document.getElementById('formulario');
                document.getElementById('submeter').value = 'excluir';
                document.getElementById('preid').value = id;

                formulario.submit();
            } else {
                return false;
            }
        }
    </script>