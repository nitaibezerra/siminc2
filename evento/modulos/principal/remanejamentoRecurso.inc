<?php

if($_REQUEST['requisicao']=="inserirRemanejamentoRecurso") {

        $eveid              = $_REQUEST['eveid'] != '' ? $_REQUEST['eveid'] : 'NULL';
        $uccdesclancamento  = $_REQUEST['uccdesclancamento'];
        $uccvalorlancamento = str_replace( array(".",","), array("","."), $_REQUEST['uccvalorlancamento'] );
        $ureidfilho         = $_REQUEST['ureidfilho'];

	$sql = "
            INSERT INTO evento.unidadecontacorrente(
                    ureidpai,
                    eveid,
                    uccdesclancamento,
                    uccvalorlancamento,
                    ureidfilho,
                    uccdatalancamento,
                    ucccpf
                ) VALUES (
                    {$_REQUEST['ureidpai']},
                    {$eveid},
                    '{$uccdesclancamento}',
                    '{$uccvalorlancamento}',
                    {$ureidfilho},
                    'NOW()',
                    '{$_SESSION['usucpf']}'
            );
        ";

	$db->executar($sql);

	$sql = "UPDATE evento.unidaderecurso SET urevalorsaldo=urevalorsaldo-'".str_replace(array(".",","),array("","."),$_REQUEST['uccvalorlancamento'])."' WHERE ureid='".$_REQUEST['ureidpai']."';";
	$db->executar($sql);
	$sql = "UPDATE evento.unidaderecurso SET urevalorsaldo=urevalorsaldo+'".str_replace(array(".",","),array("","."),$_REQUEST['uccvalorlancamento'])."' WHERE ureid='".$_REQUEST['ureidfilho']."';";
	$db->executar($sql);

	$db->commit();

	echo "
            <script>
                alert('Remanejamento de Recurso feito com sucesso');
                window.location='evento.php?modulo=principal/remanejamentoRecurso&acao=A';
            </script>
        ";
	exit;

}


if($_REQUEST['validarRemanejamentoValores']) {

	$sql = "SELECT urevalorrecurso FROM evento.unidaderecurso WHERE ureid='".$_REQUEST['ureidpai']."'";
	$vlrpai = $db->pegaUm($sql);

	if($vlrpai >= str_replace(array(".",","),array("","."),$_REQUEST['uccvalorlancamento'])) echo "TRUE";
	else echo "FALSE";

	exit;

}

if($_REQUEST['carregarComboUnidadeGestora']) {

	if($_REQUEST['preid']) {

		$sql = "SELECT ur.ureid as codigo, ung.ungdsc || '( ' || to_char(urevalorrecurso,'999g999g999g999d99') || ' )' as descricao FROM public.unidadegestora ung
				INNER JOIN evento.unidaderecurso ur ON ur.ungcod = ung.ungcod
				WHERE ur.preid='".$_REQUEST['preid']."'";

	} else {

		$sql = array();
	}

	$db->monta_combo($_REQUEST['name'], $sql, 'S', "Selecione...", $_REQUEST['acao'], '', '', '', 'S', $_REQUEST['name']);
	exit;
}

if($_REQUEST['carregarComboEventos']) {
    if($_REQUEST['ureid']) {
        $sql = "SELECT eveid as codigo, evetitulo as descricao FROM evento.evento WHERE ureid='".$_REQUEST['ureid']."'";
    } else {
        $sql = array();
    }
    $db->monta_combo('eveid', $sql, 'S', "Selecione...", '', '', '', '400', 'N', 'eveid');
    exit;
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( "Eventos", 'Remanejamento de Recurso' );
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form name = "formulario" method="post" id="formulario">
<input type="hidden" name="requisicao" value="inserirRemanejamentoRecurso">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         	<tr>
                <td class ="SubTituloDireita">Selecione o pregão:</td>
                <td>
                <?php
                $sql = "SELECT
                            preid AS codigo,
                            precodpregao || ' - ' || predescpregao AS descricao
                        FROM
                        	evento.pregaoevento
                        ORDER BY descricao";

                $db->monta_combo('preid', $sql, 'S', "Selecione...", 'carregarUnidadesGestoras', '', '', '', 'S', 'preid');
                ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Unidade Gestora (Origem):</td>
                <td id="td_unidadegestoraorigem"><?php $db->monta_combo('ureidpai', $sql=array(), 'S', "Selecione...", '', '', '', '', 'S', 'ureidpai'); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Unidade Gestora (Destino):</td>
                <td id="td_unidadegestoradestino"><?php $db->monta_combo('ureidfilho', $sql=array(), 'S', "Selecione...", '', '', '', '', 'S', 'ureidfilho'); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Evento (Destino):</td>
                <td id="td_eventodestino"><?php $db->monta_combo('eveid', $sql=array(), 'S', "Selecione...", '', '', '', '', 'N', 'eveid'); ?>
                </td>
            </tr>
             <tr>
                <td class ="SubTituloDireita" align="right">Valor(R$):</td>
                <td>
             	<?= campo_texto('uccvalorlancamento', 'S', 'S', '', 20, 20, '###.###.###.###,##', '', 'left', '',  0, 'id="uccvalorlancamento" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
             <tr>
                <td class ="SubTituloDireita" align="right">Justificativa:</td>
                <td><? echo campo_textarea( 'uccdesclancamento', 'S', 'S', '', '70', '4', '100'); ?></td>
            </tr>
            <tr>
          	<td colspan="2" class ="SubTituloDireita">
            	<input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="gravarRemanejamento();">
            	<input type="button" name="btnCancel" value="Cancelar" id="btnCancel" onclick="validaForm('cancel');">
            </td>

            </tr>
</table>
</form>

    <?PHP
        $sql = "
            SELECT  pe.precodpregao||' - '||pe.predescpregao as pregao,
                    ug.ungdsc as unorigem,
                    ug2.ungdsc as undestino,
                    uc.uccvalorlancamento,
                    us.usunome,
                    to_char(uccdatalancamento,'dd/mm/YYYY') as uccdatalancamento

            FROM evento.unidadecontacorrente uc

            INNER JOIN evento.unidaderecurso ur ON ur.ureid = uc.ureidpai
            INNER JOIN evento.pregaoevento pe ON pe.preid = ur.preid
            INNER JOIN public.unidadegestora ug ON ug.ungcod = ur.ungcod
            INNER JOIN evento.unidaderecurso ur2 ON ur2.ureid = uc.ureidfilho
            INNER JOIN public.unidadegestora ug2 ON ug2.ungcod = ur2.ungcod
            INNER JOIN seguranca.usuario us ON us.usucpf = uc.ucccpf
        ";
        $cabecalho = array("Pregão","Unidade Gestora(Origem)","Unidade Gestora(Destino)","Valor(R$)","Usuário Responsável","Data da Transferência");
        $db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);

    ?>

<div id="erro"></div>
<script type="text/javascript">

function gravarRemanejamento() {

	if(document.getElementById('ureidpai').value=='') {
		alert('Selecione Unidade Gestora (Origem)');
		return false;
	}

	if(document.getElementById('ureidfilho').value=='') {
		alert('Selecione Unidade Gestora (Destino)');
		return false;
	}

	/* COMENTADO A PEDIDO DO ANALISTA NA DATA DE 17/02/2014
        if(document.getElementById('eveid').value=='') {
		alert('Selecione Evento (Destino)');
		return false;
	}
        */
	if(document.getElementById('uccvalorlancamento').value=='') {
		alert('Valor(R$) em branco');
		return false;
	}


	if(document.getElementById('uccdesclancamento').value=='') {
		alert('Justificativa em branco');
		return false;
	}

	var validarremanejamento='';

	new Ajax.Request('evento.php?modulo=principal/remanejamentoRecurso&acao=A', {

					method: 'post',
	 				parameters: '&name=ureidpai&validarRemanejamentoValores=true&ureidpai='+document.getElementById('ureidpai').value+'&ureidfilho='+document.getElementById('ureidfilho').value+'&uccvalorlancamento='+document.getElementById('uccvalorlancamento').value,
					asynchronous: false,
			        onComplete: function (res)
			        {
			        	validarremanejamento=res.responseText;
			        }
	});

	if(validarremanejamento=="FALSE") {
		alert('Valor(R$) requerido é maior do que o saldo da Unidade de Recurso');
		return false;
	}

	document.getElementById('formulario').submit();

}

function carregarUnidadesGestoras(preid){

	var tdUGO = document.getElementById('td_unidadegestoraorigem');
	var tdUGD = document.getElementById('td_unidadegestoradestino');

	new Ajax.Request('evento.php?modulo=principal/remanejamentoRecurso&acao=A', {

					method: 'post',
	 				parameters: '&name=ureidpai&carregarComboUnidadeGestora=true&preid='+preid,
					asynchronous: false,
			        onComplete: function (res)
			        {
						tdUGO.innerHTML = res.responseText;
			        }
	});

	new Ajax.Request('evento.php?modulo=principal/remanejamentoRecurso&acao=A', {

					method: 'post',
	 				parameters: '&acao=carregarEventos&name=ureidfilho&carregarComboUnidadeGestora=true&preid='+preid,
					asynchronous: false,
			        onComplete: function (res)
			        {
						tdUGD.innerHTML = res.responseText;
			        }
	});


}

function carregarEventos(ureid){

	var tdEVD = document.getElementById('td_eventodestino');

	new Ajax.Request('evento.php?modulo=principal/remanejamentoRecurso&acao=A', {

					method: 'post',
	 				parameters: '&name=ureidpai&carregarComboEventos=true&ureid='+ureid,
					asynchronous: false,
			        onComplete: function (res)
			        {
						tdEVD.innerHTML = res.responseText;
			        }
	});

}

</script>