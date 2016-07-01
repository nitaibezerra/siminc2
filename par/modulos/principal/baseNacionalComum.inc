<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$arrDoc = array(
    0 => array("codigo" => 3, "descricao" => "Proposta Padagógica"),
    1 => array("codigo" => 4, "descricao" => "Orientação Curicular"),
    2 => array("codigo" => 5, "descricao" => "Grade Curricular")
);

if (!$_SESSION['par']['inuid']) {
    die("<script>
				alert('Problemas com variáveis. Clique a navegação novamente.');
				window.location='par.php?modulo=inicio&acao=C';
			 </script>");
} else {
    $inuid = $_SESSION['par']['inuid'];
}

$sql = "select bncid, bncperg5 from par.basenacionalcomum where inuid = {$inuid}";
$dados = $db->pegaLinha($sql);

if($dados['bncid'] && !$dados['bncperg5']){

    $dados = array(
        'alert' => 'Caro dirigente!\n\nAgradecemos sua contribuição ao responder o questionário e enviar, quando disponível, os documentos que tratam sobre o currículo da rede ou sistema sob sua responsabilidade. A partir das informações que recebemos, estamos preparando um conjunto de ações para a construção da Base Nacional Comum Curricular do Brasil. Essas ações abrangem a disponibilização de informações que coletamos bem como a realização de ampla discussão nacional.\n\nDiretoria de Currículos e Educação Integral\nSecretaria de Educação Básica\nMinistério da Educação',
        'location' => 'par.php?modulo=principal/planoTrabalho&acao=A',
    );

    die("<script>
                " . (($dados['alert']) ? "alert('" . $dados['alert'] . "');" : "") . "
                " . (($dados['location']) ? "window.location='" . $dados['location'] . "';" : "") . "
                " . (($dados['javascript']) ? $dados['javascript'] : "") . "
             </script>");
}


if ($_REQUEST['add_arquivo']) {
    ob_clean();
    $pergunta = $_REQUEST['pergunta'];
    ?>

    <tr>
        <td nowrap="nowrap">Tipo de Arquivo:
            <?php $db->monta_combo("tduid" . $pergunta . '[]', $arrDoc, 'S', 'Selecione...', '', '', '', '', 'S', 'tduid' . $pergunta); ?>
        </td>
        <td>Arquivo:</td>
        <td  nowrap="nowrap"><input type="file" name="arquivo<?php echo $pergunta; ?>[]" id="arquivo<?php echo $pergunta; ?>"/></td>
    </tr>

    <?php die;
}

if ($_REQUEST['req']) {
    $_REQUEST['req']();
    die();
}

$caminhoAtual = "par.php?modulo=principal/baseNacionalComum&acao=A";

if ($_GET['download'] == 'S') {
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
    echo"<script>window.location.href = {$caminhoAtual}';</script>";
    exit;
}

if ($_GET['requisicao'] == 'excluir') {

    $sql = "UPDATE par.basenacionalcomumarquivo SET bnastatus = 'I' WHERE arqid = {$_GET['arqid']} and bncid = {$_GET['bncid']}";
    $db->executar($sql);

    $db->commit();

    echo '<script>
			alert("Documento excluído com sucesso!");
			document.location.href = \'' . $caminhoAtual . '\';
		  </script>';
    exit;
}

if ($_REQUEST['salvar'] == '1') {
	
    if ($inuid) {

        $sql = "SELECT * FROM par.basenacionalcomum WHERE inuid = " . $inuid;
        $dados = $db->pegaLinha($sql);

        if ($dados['bncid']) { // UPDATE
            $sql = "UPDATE par.basenacionalcomum SET bncperg1 = '{$_POST['perg1']}', bncperg2 = '{$_POST['perg2']}', bncperg3 = '{$_POST['perg3']}', bncperg4 = '{$_POST['perg4']}', bncperg5 = '{$_POST['perg5']}' WHERE inuid = " . $inuid;
            $db->executar($sql);
            $bncid = $dados['bncid'];
        } else { // INSERT
            $sql = "INSERT INTO par.basenacionalcomum ( inuid, bncperg1, bncperg2, bncperg3, bncperg4, bncperg5 ) VALUES ( {$inuid}, '{$_POST['perg1']}', '{$_POST['perg2']}', '{$_POST['perg3']}', '{$_POST['perg4']}', '{$_POST['perg5']}' ) RETURNING bncid";
            $bncid = $db->pegaUm($sql);
        }
        $files = $_FILES;
        for($i=1; $i<=5; $i++){

            unset($_FILES);
            if(isset($_POST['tduid' . $i]) && is_array($_POST['tduid' . $i])){

                foreach ($_POST['tduid' . $i] as $key => $tduid) {

                    if ($files['arquivo' . $i] && !empty($tduid) ) {
                        if ($files['arquivo' . $i]['error'][$key] == 0) {
                            $campos = array(
                                "tipoDocumento" => $tduid,
                                "bncid" => $bncid,
                                "bnadataatualizacao" => "NOW()",
                                "bnaperg" => $i,
                                "usucpf" => "'{$_SESSION['usucpf']}'"
                            );

                            $_FILES['arquivo'] = array(
                                'name' => $files['arquivo' . $i]['name'][$key],
                                'type' => $files['arquivo' . $i]['type'][$key],
                                'tmp_name' => $files['arquivo' . $i]['tmp_name'][$key],
                                'error' => $files['arquivo' . $i]['error'][$key],
                                'size' => $files['arquivo' . $i]['size'][$key],
                            );

                            $file = new FilesSimec("basenacionalcomumarquivo", $campos, "par");
                            $file->setUpload('Base_nacional', null, true, '');
                        }
                    }
                }
            }
        }

//        $db->commit();
        echo"<script> alert('Operação realizada com sucesso!'); window.location.href = window.location.href;</script>";
    } else {
        die("<script>
				alert('Problemas com variáveis. Clique a navegação novamente.');
				window.location='par.php?modulo=inicio&acao=C';
			 </script>");
    }
}

$sql = "SELECT * FROM par.basenacionalcomum WHERE inuid = " . $inuid;
$dadoBase = $db->pegaLinha($sql);

if ($dadoBase['bncperg1'] != "") {
    ${"p1" . $dadoBase['bncperg1']} = 'checked="checked"';
}
if ($dadoBase['bncperg2'] != "") {
    ${"p2" . $dadoBase['bncperg2']} = 'checked="checked"';
}
if ($dadoBase['bncperg3'] != "") {
    ${"p3" . $dadoBase['bncperg3']} = 'checked="checked"';
}
if ($dadoBase['bncperg4'] != "") {
    ${"p4" . $dadoBase['bncperg4']} = 'checked="checked"';
}
if ($dadoBase['bncperg5'] != "") {
    ${"p5" . $dadoBase['bncperg5']} = 'checked="checked"';
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

$pergunta = 1;
$linhaAddArquivo = '<tr>
        <td nowrap="nowrap">Tipo de Arquivo:
            $db->monta_combo("tduid'. $pergunta . '[]", $arrDoc, $valida, "Selecione...", "", "", "", "", "S", "tduid"'. $pergunta . ');
        </td>
        <td>Arquivo:</td>
        <td  nowrap="nowrap"><input type="file" name="arquivo'. $pergunta . '[]" id="arquivo'. $pergunta . '"/></td>
    </tr>';

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/par.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript">

    jQuery.noConflict();

    jQuery(function(){

        if('a' == jQuery('input[name=perg1]:checked').val() ){ jQuery('#arquivo_perg1').show() }
        if('a' == jQuery('input[name=perg2]:checked').val() ){ jQuery('#arquivo_perg2').show() }
        if('a' == jQuery('input[name=perg3]:checked').val() ){ jQuery('#arquivo_perg3').show() }
        if('a' == jQuery('input[name=perg4]:checked').val() ){ jQuery('#arquivo_perg4').show() }
        if('a' == jQuery('input[name=perg5]:checked').val() ){ jQuery('#arquivo_perg5').show() }

        jQuery('input[name^=perg]').click(function(){
            var div = 'arquivo_' + jQuery(this).attr('name');
            var valor = jQuery(this).val();

            'a' == valor ? jQuery('#'+div).show() : jQuery('#'+div).hide();
        });


        jQuery('.add_arquivo').click(function(){
            var pergunta = jQuery(this).attr('data-pergunta');

            jQuery.ajax({
                url: 'par.php?modulo=principal/baseNacionalComum&acao=A&add_arquivo=1&pergunta=' + pergunta,
                success:function(result){
                    jQuery('#tabela_arquivos_'+pergunta).append(result);
                }
            });

            jQuery('#tabela_arquivos_'+pergunta).load();
        });

    });


    function excluirDocumento(url, arqid, bncid)
    {
        if (boAtivo = 'S')
        {
            if (confirm("Deseja realmente excluir este Documento?")) {
                window.location = url + '&arqid=' + arqid + '&bncid=' + bncid;
            }
        }
    }

    function downloadArquivo(arqid)
    {

        window.location = '?modulo=principal/baseNacionalComum&acao=A&download=S&arqid=' + arqid

    }

    function enviar()
    {
        var testa1 = false;
        var testa2 = false;
        var testa3 = false;
        var testa4 = false;
        var testa5 = false;

//        var flagSim = false;
        var flagSim1 = false;
        var flagSim2 = false;
        var flagSim3 = false;
        var flagSim4 = false;
        var flagSim5 = false;

        var perg1 = document.getElementsByName('perg1');
        var perg2 = document.getElementsByName('perg2');
        var perg3 = document.getElementsByName('perg3');
        var perg4 = document.getElementsByName('perg4');
        var perg5 = document.getElementsByName('perg5');

//        var arq = document.getElementById('arquivo');

        var arq1 = document.getElementById('arquivo1');
        var arq2 = document.getElementById('arquivo2');
        var arq3 = document.getElementById('arquivo3');
        var arq4 = document.getElementById('arquivo4');
        var arq5 = document.getElementById('arquivo5');

        var tipoDocunid1 = document.getElementById('tduid1');
        var tipoDocunid2 = document.getElementById('tduid2');
        var tipoDocunid3 = document.getElementById('tduid3');
        var tipoDocunid4 = document.getElementById('tduid4');
        var tipoDocunid5 = document.getElementById('tduid5');

        for (var i = 0; i < perg1.length; i++) {
            if (perg1[i].checked) {
                testa1 = true;
                if (perg1[i].value === 'a') {
                    flagSim1 = true;
                }
            }
            if (perg2[i].checked) {
                testa2 = true;
                if (perg2[i].value === 'a') {
                    flagSim2 = true;
                }
            }
            if (perg3[i].checked) {
                testa3 = true;
                if (perg3[i].value === 'a') {
                    flagSim3 = true;
                }
            }
            if (perg4[i].checked) {
                testa4 = true;
                if (perg4[i].value === 'a') {
                    flagSim4 = true;
                }
            }
            if (perg5[i].checked) {
                testa5 = true;
                if (perg5[i].value === 'a') {
                    flagSim5 = true;
                }
            }
        }

        if (testa1 === false) {
            alert('É necessário responder a questão 01.');
            return false;
        }
        if (testa2 === false) {
            alert('É necessário responder a questão 02.');
            return false;
        }
        if (testa3 === false) {
            alert('É necessário responder a questão 04.');
            return false;
        }
        if (testa4 === false) {
            alert('É necessário responder a questão 03.');
            return false;
        }
        if (testa5 === false) {
            alert('É necessário responder a questão 05.');
            return false;
        }

        if (flagSim1 && document.getElementById('possuianexo1').value == '') {
            if (arq1.value == ''){
            alert('É necessário anexar pelo menos um arquivo (Item 1).');
            return false;
            }
            if (tipoDocunid1.value == '')
            {
                alert('É necessário informar o Tipo de Arquivo (Item 1).');
                return false;
            }
        }

        if (flagSim2 && document.getElementById('possuianexo2').value == '') {
            if (arq2.value == ''){
            alert('É necessário anexar pelo menos um arquivo (Item 2).');
            return false;
            }
            if (tipoDocunid2.value == '')
            {
                alert('É necessário informar o Tipo de Arquivo (Item 2).');
                return false;
            }
        }

        if (flagSim3 && document.getElementById('possuianexo3').value == '') {
            if (arq3.value == ''){
            alert('É necessário anexar pelo menos um arquivo. (Item 4)');
            return false;
            }
            if (tipoDocunid3.value == '')
            {
                alert('É necessário informar o Tipo de Arquivo. (Item 4)');
                return false;
            }
        }

        if (flagSim4 && document.getElementById('possuianexo4').value == '') {
            if (arq4.value == ''){
            alert('É necessário anexar pelo menos um arquivo. (Item 3)');
            return false;
            }
            if (tipoDocunid4.value == '')
            {
                alert('É necessário informar o Tipo de Arquivo. (Item 3)');
                return false;
            }
        }

        if (flagSim5 && document.getElementById('possuianexo5').value == '') {
            if (arq5.value == ''){
            alert('É necessário anexar pelo menos um arquivo (Item 5).');
            return false;
            }
            if (tipoDocunid5.value == '')
            {
                alert('É necessário informar o Tipo de Arquivo (Item 5).');
                return false;
            }
        }

        document.formulario.salvar.value = 1;
        document.formulario.submit();
    }
</script>
<?php
// Este parâmetro ($arrAbas) irá servir para retirar a aba "Acompanhamento" que só deverá estar visível quando ela mesma estiver ativa
$arrAbas = array(PAR_ESCONDE_ABA_ACOMPANHAMENTO);
$db->cria_aba($abacod_tela, $url, '', $arrAbas);

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']);
monta_titulo($nmEntidadePar, '');

//PAR
monta_titulo($titulo_modulo, 'POLÍTICA CURRICULAR PARA A EDUCAÇÃO BÁSICA');
//
$perfil = pegaArrayPerfil($_SESSION['usucpf']);

//CONDIÇÃO QUE DETERMINA OS PERFIS QUE PODEM EDITAR A TELA DA BASE NACIONAL.  
if ((in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
        in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
        in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
        in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil))) {
    $disable = FALSE;
} else {
    $disable = TRUE;
}

if (!$disable) {
    $btnExcluir = "<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"javascript:excluirDocumento(\'" . $caminhoAtual . "&requisicao=excluir" . "\',' || bna.arqid || ',' || bna.bncid || ');\" >";
}

//if(!) {
?>

<table width="100%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">

    <tr>

        <td class="SubTitulocentro" >

        </td>

    </tr>

    <?php ?><tr>
        <td > 
            <form name=formulario id=formulario method=post enctype="multipart/form-data">
                <input type="hidden" name="salvar" value="0">
                <table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tr>
                        <td align="justify" style="font-family: Calibri;font-size:15px"><b>Caro Dirigente,</b><br/><br/>

                            A <b>Diretoria de Currículos e Educação Integral</b> da <b>Secretaria de Educação Básica</b> está realizando um levantamento sobre as 
                            <b>Propostas Curriculares</b> construídas nos sistemas públicos de ensino (estados, municípios e Distrito Federal) a partir da homologação das novas 
                            <b>Diretrizes Curriculares Nacionais</b> nos anos de 2009 para a <b>Educação Infantil</b>, 2010 para o <b>Ensino Fundamental</b> e 2012 para o 
                            <b>Ensino Médio</b>. Para tanto, solicitamos sua contribuição respondendo algumas perguntas e, nos casos de existência de proposta curricular, 
                            enviando os arquivos digitais das mesmas. Esclarecemos, também, que as propostas enviadas servirão de subsídios necessários à construção coletiva 
                            de uma <b>Base Nacional Comum</b> para o currículo da Educação Básica e <b>serão divulgados</b> em site do Ministério da Educação sobre a Política 
                            Curricular Nacional. A contribuição de V. Sa. é de grande importância para que possamos avançar nesse importante tema de debate nacional.<br><br>
                            
                            <span style="color: red;">IDENTIFICAÇÃO GERAL<br></span>
                            - Unidade da Federação: <b><?php echo $nmEntidadePar ?></b><br>
                            - Sistema de Ensino: <b><?php echo (($_SESSION['par']['itrid'] == 1) ? 'Estadual' : 'Municipal'); ?></b> <br><br>


                            <div style="border: 1px red solid; text-align: justify; padding: 5px 10px; margin-bottom: 20px;">
                                <p>Por favor, leia atentamente cada uma das perguntas a seguir. No caso de resposta afirmativa <b>(SIM)</b> pedimos que seja(m) anexado(s) o(s) documento(s) referente(s) à etapa ou modalidade da Educação Básica referida na pergunta. Ao final, não esqueça de clicar no botão SALVAR para que as informações sejam enviadas ao MEC. Use a legenda abaixo para classificar os documentos enviados.</p>
                                <p><b>PROPOSTA PEDAGÓGICA</b>: A proposta pedagógica é a identidade da escola, é o documento que descreve as diretrizes de ensino e aprendizagem. Ela formaliza um compromisso assumido por professores, funcionários, representantes de pais, alunos e comunidade em torno do mesmo propósito de efetivar o projeto educacional da escola.</p>
                                <p><b>ORIENTAÇÕES CURRICULARES</b>: As orientações Curriculares são diretrizes traçadas pelos conselhos municipais, estaduais ou nacional, com princípios, fundamentos e procedimentos na Educação Básica que orientam as escolas na organização, articulação, desenvolvimento e avaliação de suas propostas pedagógicas. Geralmente é um documento normativo. Apontam as expectativas do que os alunos precisam aprender em cada modalidade ou cada ano. Aborda todas as áreas do conhecimento. São contemplados tanto conteúdos conceituais como procedimentos necessários aos alunos para progredir em seu aprendizado. Devem ser lidas e interpretadas pelos diferentes profissionais que integram o processo educacional.</p>
                                <p><b>GRADE CURRICULAR</b>: A grade curricular define a quantidade de aulas para cada modalidade, por ano, semestre, mês, semana ou dia, observando a carga horária mínima obrigatória de cada componente curricular ou disciplina, a ser definida pela Secretaria de Educação ou pela escola em reunião pedagógica, para posterior aprovação da Secretaria.</p>
                            </div>


                            <?php
                                $aPerguntas = array(
                                    '1' => 'O Sistema de Ensino sob sua responsabilidade possui orientações curriculares, ou matriz curricular, ou outra modalidade de documento referente à organização do currículo na EDUCAÇÃO INFANTIL elaboradas a partir de dezembro de 2009?',
                                    '2' => 'O Sistema de Ensino sob sua responsabilidade possui orientações curriculares, ou matriz curricular, ou outra modalidade de documento referente à organização do currículo no ENSINO FUNDAMENTAL - ANOS INICIAIS elaboradas a partir de julho de 2010?',
                                    '4' => 'O Sistema de Ensino sob sua responsabilidade possui orientações curriculares, ou matriz curricular, ou outra modalidade de documento referente à organização do currículo no ENSINO FUNDAMENTAL - ANOS FINAIS elaboradas a partir de julho de 2010?',
                                    '3' => 'O Sistema de Ensino sob sua responsabilidade possui orientações curriculares, ou matriz curricular, ou outra modalidade de documento referente à organização do currículo no ENSINO MÉDIO elaboradas a partir de julho de 2010?',
                                    '5' => 'O Sistema de Ensino sob sua responsabilidade possui orientações curriculares, ou matriz curricular, ou outra modalidade de documento referente à organização do currículo na EDUCAÇÃO DE JOVENS E ADULTOS elaboradas a partir de julho de 2010?',
                                );

                                $aRespostas = array(
                                    'a' => 'a. Sim',
                                    'b' => 'b. não',
                                    'c' => 'c. não, mas está em elaboração',
                                    'd' => 'd. não possuo informações sobre o assunto',
                                );
                                $numero = 0;
                                foreach ($aPerguntas as $pergunta => $texto) {
                                    $numero++; ?>
                                    <b><?php echo $numero; ?></b> <?php echo $texto; ?><br>

                                    <div style="float: left;">
                                        <?php foreach ($aRespostas as $resposta => $textoResp) {
                                            $valor = "p{$pergunta}{$resposta}"; 
//                                            ver($valor, $$valor);
                                            ?>
                                            <div>
                                                <input type="radio" id="perg<?php echo $pergunta . $resposta; ?>" <?php if ($disable) { echo "disabled='disabled'"; } ?> name="perg<?php echo $pergunta; ?>" value="<?php echo $resposta; ?>" <?= $$valor; ?>> <?php echo $textoResp; ?><br>
                                            </div>
                                        <?php } ?>
                                    </div>
                                    <div style="float: left; margin-left: 50px; display: none;" id="arquivo_perg<?php echo $pergunta; ?>">
                                        <table id="tabela_arquivos_<?php echo $pergunta; ?>">
                                            <tr>
                                                <td nowrap="nowrap">Tipo de Arquivo:
                                                    <?php $db->monta_combo("tduid" . $pergunta . '[]', $arrDoc, 'S', 'Selecione...', '', '', '', '', 'S', 'tduid' . $pergunta); ?>
                                                </td>
                                                <td>Arquivo:</td>
                                                <td  nowrap="nowrap"><input type="file"  <?php if ($disable) { echo "disabled='disabled'"; } ?> name="arquivo<?php echo $pergunta; ?>[]" id="arquivo<?php echo $pergunta; ?>"/></td>
                                            </tr>
                                        </table>
                                        <input type="button" name="add_arquivo" class="add_arquivo" value="Mais arquivos" data-pergunta="<?php echo $pergunta; ?>" />
                                    </div>
                                    <div style="clear: both;"></div>

                                    <?php
                                    $sql = "
                                            SELECT
                                                    '<img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"downloadArquivo('|| bna.arqid ||');\">'
                                                    ||
                                                    '{$btnExcluir}'
                                                    as acao,
                                                    CASE
                                                        WHEN tipodocumento = 3 THEN 'Proposta Padagógica'
                                                        WHEN tipodocumento = 4 THEN 'Orientação Curicular'
                                                        ELSE 'Grade Curricular'
                                                    END as descricao,
                                                    usu.usunome
                                            FROM
                                                    par.basenacionalcomum bnc
                                            INNER JOIN par.basenacionalcomumarquivo bna ON bna.bncid = bnc.bncid AND bnastatus = 'A'
                                            INNER JOIN seguranca.usuario usu on bna.usucpf = usu.usucpf
                                            WHERE inuid = $inuid
                                            and bnaperg = {$pergunta}";

                                    $dados = $db->carregar($sql);
                                    if($dados){
                                        echo '<h5 style="color: red; text-align: center;">Documentos Anexados</h5>';
                                        $cabecalho = array('Ação', 'Tipo de Documento Unidade', 'Inserido por');
                                        $db->monta_lista($dados, $cabecalho, 20, 10, 'N', '', '');
                                        echo '<br/>';
                                    }
                                    ?>
                                    <input type="hidden" name="possuianexo" id="possuianexo<?php echo $pergunta; ?>" value="<?php echo $dados ? 1 : ''; ?>" />
                                    <br />
                                <?php }
                            ?>
                            <br>
                            <div style="clear: both;"></div>
                        </td>
                    </tr>
                </table>
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">	
                    <tr id="tr_termos" style="display:none">
                        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Termos de compromisso:</td>
                        <td width='50%' id="td_termos">
                        </td>      
                    </tr>
                    <tr style="background-color: #cccccc">
                        <td>&nbsp;</td>
                        <td><input type="button" name="botao" value="Salvar"  <?php
            if ($disable) {
                echo "disabled='disabled'";
            }
            ?> onclick="enviar()" <?= $disableSalvar ?>></td>
                    </tr> 
                </table>
            </form>

        </td>

    </tr>
</table>


