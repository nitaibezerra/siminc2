<?
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include "relatorio_monitoramento_resultado.inc";
$superuser = $db->testa_superuser();
$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql($superuser);
$dados = $db->carregar($sql);

if( $_POST['req'] == 'geraxls' ){
	$arCabecalho = array();
	$colXls = array();
	array_push($arCabecalho, 'A escola começou a preencher a aba Monitoramento?');
	array_push($colXls, 'monitoramento');
	array_push($arCabecalho, 'Resposta');
	array_push($colXls, 'preenchimento');
	array_push($arCabecalho, 'Estado');
	array_push($colXls, 'estuf');
	array_push($arCabecalho, 'Município');
	array_push($colXls, 'municipio');
	array_push($arCabecalho, 'Escola');
	array_push($colXls, 'escola');
	foreach($col as $cabecalho){
		array_push($arCabecalho, $cabecalho['label']);
		array_push($colXls, $cabecalho['campo']);
	}
	
	$arDados = Array();
	if( is_array( $dados ) ){
		foreach( $dados as $k => $registro ){
			foreach( $colXls as $campo ){
				$arDados[$k][$campo] = $registro[$campo];
			}
		}
	}
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatorioIndicadores".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%',$par2);
	die;
}

include APPRAIZ . 'includes/Agrupador.php';
include  APPRAIZ . 'includes/cabecalho.inc';
?>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>
   	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	

<?php
echo '<br />';
monta_titulo('', 'Relatório Preenchimento Monitoramento');

$bloq = 'S';
$disabled = '';

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function()
{
//jQuery('[name="status"]').click(function(){
//	if( jQuery(this).val() == 'I' ){
//		jQuery('#tr_justificativa').show();
//		jQuery('#tr_caejustificativainativacao').show();
//	}else{
//		jQuery('#caejustificativainativacao').val('');
//		jQuery('#tr_caejustificativainativacao').hide();
//		jQuery('#tr_justificativa').hide();
//		jQuery('[name="justificativaD"]').attr('checked',false);
//		jQuery('[name="justificativaF"]').attr('checked',false);
//		jQuery('[name="justificativaO"]').attr('checked',false);
//	}
//});
});

    function exibeRelatorioProcesso(tipoXLS) {
        document.getElementById('req').value = tipoXLS;
        var formulario = document.formulario;

        if (document.getElementById('req').value != 'geraxls') {
            formulario.submit();
//             janela.focus();
        } else {
            formulario.submit();
        }
    }

    /* Função para substituir todos */
    function replaceAll(str, de, para) {
        var pos = str.indexOf(de);
        while (pos > -1) {
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }



    /**
     * Alterar visibilidade de um bloco.
     * 
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco(bloco)
    {
        var div_on = document.getElementById(bloco + '_div_filtros_on');
        var div_off = document.getElementById(bloco + '_div_filtros_off');
        var img = document.getElementById(bloco + '_img');
        var input = document.getElementById(bloco + '_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }

    /**
     * Alterar visibilidade de um campo.
     * 
     * @param string indica o campo a ser mostrado/escondido
     * @return void
     */
    function onOffCampo(campo)
    {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }
</script>

<form action="" method="post" name="formulario" id="filtro">
    <input type="hidden" name="form" value="1"/>
    <input type="hidden" id="req" name="req" value=""/>	
    <input type="hidden" name="pesquisa" value="1"/>
    <input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
    <input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
    <input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
    <input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
            <td bgcolor="#CCCCCC" align='center'>
                <input type="button" value="VisualizarXLS" onclick="exibeRelatorioProcesso('geraxls');">
            </td>
		</tr>
    	<tr>
    		<td>
    		<?php 
	    		$r = new montaRelatorio();
	    		$r->setAgrupador($agrup, $dados);
	    		$r->setColuna($col);
	    		$r->setTotNivel(true);
	    		$r->setMonstrarTolizadorNivel(false);
	    		$r->setTotalizador(false);
	    		$r->setBrasao(true);
	    		$r->setEspandir( $_REQUEST['expandir']);
	    		echo $r->getRelatorio();
    		?>
    		</td>
    	</tr>
	</table>
</form>
<?
?>