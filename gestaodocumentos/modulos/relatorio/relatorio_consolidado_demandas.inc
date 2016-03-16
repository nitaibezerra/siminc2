<?php

if ($_POST['agrupador']){
    ini_set("memory_limit","2048M");
    $true = true;
    include("relatorio_consolidado_demandas_resultado.inc");
    exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br>';

monta_titulo( 'Relatório Consolidado Demandas SERES', 'Gestão de Demandas SERES' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/calendario.js"></script>

<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">
    function gerarRelatorio(req){
        var formulario = document.formulario;

        document.getElementById('req').value = req;

        if (formulario.elements['agrupador'][0] == null){
            alert('Selecione pelo menos um agrupador!');
            return false;
        }

        selectAllOptions( formulario.agrupador );
        selectAllOptions( formulario.tpsid );
        selectAllOptions( formulario.nvcid );
        selectAllOptions( formulario.esdid );
        selectAllOptions( formulario.unaid );
        selectAllOptions( formulario.temid );
        selectAllOptions( formulario.sitid );
        selectAllOptions( formulario.tpeid );
        selectAllOptions( formulario.usucpf );

        var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
        janela.opener.notclose = janela;

        formulario.target = 'relatorio';
        formulario.submit();
    }

    /*
     * Alterar visibilidade de um campo.
     *
     * @param string indica o campo a ser mostrado/escondido
     * @return void
    */
    function onOffCampo( campo ){
        var div_on = document.getElementById( campo + '_campo_on' );
        var div_off = document.getElementById( campo + '_campo_off' );
        var input = document.getElementById( campo + '_campo_flag' );
        if ( div_on.style.display == 'none' ){
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }else{
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }
</script>

</head>
<body>

<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name="req" id="req" value="" />

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
            <td class="SubTituloDireita" valign="top">Agrupadores</td>
            <td>
                <?
                    $matriz = array(
                        array('codigo' => 'solicitante', 'descricao' => '01. Tipo Solicitante'),
                        array('codigo' => 'nvcdsc', 'descricao' => '02. Nível Complexidade'),
                        array('codigo' => 'esddsc', 'descricao' => '03. Status WorkFlow'),
                        array('codigo' => 'tarporcentoexec', 'descricao' => '04. Percentual Executado'),
                        array('codigo' => 'uni_orig', 'descricao' => '05. Setor Origem'),
                        array('codigo' => 'temdescricao', 'descricao' => '06. Assunto'),
                        array('codigo' => 'tpedsc', 'descricao' => '07. Expediente'),
                        array('codigo' => 'nome_responsavel', 'descricao' => '08. Responsável'),
                        array('codigo' => 'tardatarecebimento', 'descricao' => '09. Data do Recebimento'),
                        array('codigo' => 'tardataprazoatendimento', 'descricao' => '10. Data do Atendimento'),
                        array('codigo' => 'sitdsc', 'descricao' => '11. Situação da Demanda')
                    );
                    $campoAgrupador = new Agrupador( 'formulario' );
                    $campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
                    $campoAgrupador->setDestino( 'agrupador', null, null);
                    $campoAgrupador->exibir();
                ?>
            </td>
	</tr>
        <tr>
            <td class="SubTituloDireita">Data do Recebimento:</td>
            <td>
                <?php
                    //$filtrotardtinclusao_1 = $_SESSION['GD_filtro']['filtrotardtinclusao_1'];
                    echo campo_data2('tardtinclusao_1','N','S','','DD/MM/YYYY','','', $filtrotardtinclusao_1, '', '', 'tardtinclusao_1' ); 
                    echo " até ";
                    //$filtrotardtinclusao_2 = $_SESSION['GD_filtro']['filtrotardtinclusao_2'];
                    echo campo_data2('tardtinclusao_2','N','S','','DD/MM/YYYY','','', $filtrotardtinclusao_2, '', '', 'tardtinclusao_2' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data para Atendimento:</td>
            <td>
                <?php                    
                    //$filtroprazoini = $_SESSION['GD_filtro']['filtroprazoini'];
                    echo campo_data2('tardataprazoatendimento_1','N','S','','DD/MM/YYYY','','', $filtroprazoini, '', '', 'tardataprazoatendimento_1' ); 
                    echo " até ";
                    //$filtroprazofim = $_SESSION['GD_filtro']['filtroprazofim'];
                    echo campo_data2('tardataprazoatendimento_2','N','S','','DD/MM/YYYY','','', $filtroprazofim, '', '', 'tardataprazoatendimento_2' );
                ?>
            </td>
        </tr>
            <?PHP
                #Tipo de Solicitante
                $stSql = "
                    SELECT  tpsid AS codigo,
                            tpsdsc AS descricao
                    FROM gestaodocumentos.tiposolicitante
                    ORDER BY tpsdsc
                ";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Tipo de Solicitante', 'tpsid',  $stSql, $stSqlCarregados, 'Selecione o(s) Tipo(s) de Solicitante(s)' );

                #Nível
                $stSql = "
                    SELECT  nvcid AS codigo,
                            nvcdsc AS descricao
                    FROM gestaodocumentos.nivelcomplexidade
                    ORDER by descricao
                ";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Nível de Complexidade', 'nvcid',  $stSql, $stSqlCarregados, 'Selecione o(s) Nível(is)' );

                #Status Workflow
                $stSql = "
                    SELECT  esdid AS codigo,
                            esddsc AS descricao
                    FROM workflow.estadodocumento
                    WHERE tpdid = " . FUXO_GESTAO_DOCUMENTOS . "
                ";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Estado do WorkFlow', 'esdid',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s) do WorkFlows' );

                #Setor de Origem
                $stSql = "
                    SELECT  unaid as codigo,
                            unasigla||' - '|| unadescricao as descricao
                    FROM gestaodocumentos.unidade
                    ORDER BY unadescricao
                ";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Setor de Origem', 'unaid',  $stSql, $stSqlCarregados, 'Selecione o(s) Setor(es) de Origem(ns)' );

                #Assunto
                $stSql = "
                    SELECT  temid AS codigo,
                            temdescricao AS descricao
                    FROM gestaodocumentos.tema
                    ORDER BY descricao
                ";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Assunto', 'temid',  $stSql, $stSqlCarregados, 'Selecione o(s) Assunto(s)' );

                #situação da Demanda
                $stSql = "
                    SELECT  sitid AS codigo,
                            sitdsc AS descricao
                    FROM gestaodocumentos.situacaotarefa
                    ORDER BY codigo
                ";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Situação da Demanda', 'sitid',  $stSql, $stSqlCarregados, 'Selecione a(s) situação(ões) da Demanda' );

                #Expediente
                $stSql = "
                    SELECT  tpeid AS codigo,
                            tpedsc AS descricao
                    FROM gestaodocumentos.tipoexpediente
                    ORDER BY tpedsc
                ";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Expediente', 'tpeid',  $stSql, $stSqlCarregados, 'Selecione o(s) Expediente(s)' );

                #Responsável pela Demanda
                $stSql = "
                    SELECT  u.usucpf as codigo,
                            u.usunome as descricao
	            FROM seguranca.usuario AS u
	            LEFT JOIN 	seguranca.perfilusuario AS p ON p.usucpf = u.usucpf

		    WHERE p.pflcod IN (" . PERFIL_GDOCUMENTO_APOIO . "," . PERFIL_GDOCUMENTO_COORDENACAO_GERAL . "," . PERFIL_GDOCUMENTO_DIRETORIA . "," . PERFIL_GDOCUMENTO_ESQUIPE_TEC . "," . PERFIL_GDOCUMENTO_ADMINISTRADOR . ")
		    ORDER BY u.usunome
                ";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Responsável pela Demanda', 'usucpf',  $stSql, $stSqlCarregados, 'Selecione o(s) Responsável(is) pela(s) Demanda(s)' );
            ?>
	<tr>
            <td align="center" colspan="2">
                <input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio(1);"/>
                <input type="button" name="Gerar Relatório XLS" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio(2);"/>
            </td>
	</tr>
    </table>
</form>

</body>
</html>