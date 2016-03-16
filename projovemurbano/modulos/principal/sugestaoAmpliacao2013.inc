<?php

	function salvar(){
		
		global $db;
		
		$sql = "DELETE FROM projovemurbano.metasdoprograma WHERE cmeid = {$_POST['cmeid']} AND tpmid in (3,8,9,11,12,14,15);";
		
		$suaid = $_REQUEST['suaid'];
		if ($suaid) {
			$sql .= "UPDATE projovemurbano.sugestaoampliacao SET suaverdade = TRUE WHERE ppuid = {$_SESSION['projovemurbano']['ppuid']} AND pjuid = {$_SESSION['projovemurbano']['pjuid']};";
		} else {
			$sqlSugestao = "
				INSERT INTO projovemurbano.sugestaoampliacao (pjuid, suaverdade, suametasugerida, suastatus, suametaajustada, ppuid)
				VALUES ({$_SESSION['projovemurbano']['pjuid']}, TRUE, null, 'A', null, {$_SESSION['projovemurbano']['ppuid']}) returning suaid;
			";
			$suaid = $db->pegaUm($sqlSugestao);
		}
		$sql .= "UPDATE projovemurbano.sugestaoampliacao SET 
					suaverdade = {$_POST['suaverdade']} 
				WHERE 
					ppuid = {$_SESSION['projovemurbano']['ppuid']} 
					AND pjuid = {$_SESSION['projovemurbano']['pjuid']};";
		
		if( $_POST['mtpvalor'] && $_POST['suaverdade'] == 'TRUE' ){
			foreach( $_POST['mtpvalor'] as $tpmid => $mtpvalor ){
                if( $mtpvalor != '' ){
                    $sql .= "INSERT INTO projovemurbano.metasdoprograma(tpmid, pjuid, ppuid, suaid, cmeid, mtpvalor)
                            VALUES ($tpmid, {$_SESSION['projovemurbano']['pjuid']}, {$_SESSION['projovemurbano']['ppuid']}, $suaid, {$_POST['cmeid']}, '$mtpvalor');";
                }
			}
		}
		
		if ($sql) {
			$db->executar($sql);
			$db->commit();
		}
		
		echo "
			<script>
				alert('Sugestão de Meta gravada com sucesso.');
				window.location = 'projovemurbano.php?modulo=principal/sugestaoAmpliacao&acao=A';
			</script>
			";
	}

    if ($_REQUEST['req']) {
        $_REQUEST['req']($_REQUEST);
        exit;
    }

    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';

    echo montarAbasArray(montaMenuProJovemUrbano(), $_SERVER['REQUEST_URI']);

    monta_titulo('Projovem Urbano', montaTituloEstMun());

    $sql = "SELECT * FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION['projovemurbano']['pjuid'] . "' and ppuid = {$_SESSION['projovemurbano']['ppuid']}";
    $sugestaoampliacao = $db->pegaLinha($sql);

    if ( $sugestaoampliacao ) {
        $requisicao = "atualizarSugestaoAmpliacao";
        extract($sugestaoampliacao);
    } else {
        $requisicao = "inserirSugestaoAmpliacao";
    }

    $habilita = 'S';
    $disabilita = '';
    $perfil = pegaPerfilGeral();
    if (in_array(PFL_CONSULTA, $perfil)) {
        $habilita = 'N';
        $disabilita = 'S';
//         ver('oi');
    }

   if( $db->testa_superuser() || in_array(PFL_EQUIPE_MEC, $perfil) || in_array(PFL_ADMINISTRADOR, $perfil) || $habilita == 'S' ){
        $habilitaMetaSugerida = 'S';
   }else{
       $habilitaMetaSugerida = 'N';
   }
    
    if ($_SESSION['projovemurbano']['muncod']) {
        $cmecodibge = $_SESSION['projovemurbano']['muncod'];
    } else {
        $cmecodibge = $db->pegaUm("select estcod from territorios.estado where estuf = '{$_SESSION['projovemurbano']['estuf']}'");
    }

    $sql = "SELECT * FROM projovemurbano.cargameta WHERE cmecodibge = '{$cmecodibge}' AND ppuid = {$_SESSION['projovemurbano']['ppuid']}";
    $rsCargaMeta = $db->pegaLinha($sql);
    
    if ($_SESSION['projovemurbano']['muncod']) {
    	$cmecodibge = "AND muncod = '" . $_SESSION['projovemurbano']['muncod'] . "'";
    } else {
    	$cmecodibge = "AND estuf = '" . $_SESSION['projovemurbano']['estuf'] . "'"; //" and estuf = '".$db->pegaUm("select estuf from territorios.estado where estuf = '{$_SESSION['projovemurbano']['estuf']}'")."'";
    }
    
    $sql = "SELECT
			    a.suaid,
			    a.pjuid,
			    a.suaverdade,
			    cast(a.suametasugerida as integer) as suametasugerida,
			    a.suastatus,
			    cast(a.suametaajustada as integer) as suametaajustada,
			    a.ppuid
		    FROM
		    	projovemurbano.projovemurbano p
		    LEFT JOIN projovemurbano.sugestaoampliacao a on a.pjuid = p.pjuid
		    WHERE
                p.pjuid = {$_SESSION['projovemurbano']['pjuid']}
			    {$cmecodibge}
		    ";
    $rsSugestaoMeta = $db->pegaLinha($sql);
   
    if($rsSugestaoMeta['suaid']){
        $sql = "SELECT mtpvalor FROM projovemurbano.metasdoprograma 
                WHERE tpmid = 3 AND cmeid = {$rsCargaMeta['cmeid']} AND ppuid = {$_SESSION['projovemurbano']['ppuid']} AND suaid = {$rsSugestaoMeta['suaid']}";
        $metaAjustada = $db->pegaUm($sql);
        $metaAjustada = $metaAjustada ? $metaAjustada : '0';

        $sql = "SELECT mtpvalor FROM projovemurbano.metasdoprograma
                WHERE tpmid = 9 AND cmeid = {$rsCargaMeta['cmeid']} AND ppuid = {$_SESSION['projovemurbano']['ppuid']} AND suaid = {$rsSugestaoMeta['suaid']}";
        $metaPublicoJuventudeVivaAjustada = $db->pegaUm($sql);
        $metaPublicoJuventudeVivaAjustada = $metaPublicoJuventudeVivaAjustada ? $metaPublicoJuventudeVivaAjustada : '0';

        $sql = "SELECT mtpvalor FROM projovemurbano.metasdoprograma
                WHERE tpmid = 12 AND cmeid = {$rsCargaMeta['cmeid']} AND ppuid = {$_SESSION['projovemurbano']['ppuid']} AND suaid = {$rsSugestaoMeta['suaid']}";
        $metaPublicoUnidadesPrisionaisAjustada = $db->pegaUm($sql);
        $metaPublicoUnidadesPrisionaisAjustada = $metaPublicoUnidadesPrisionaisAjustada ? $metaPublicoUnidadesPrisionaisAjustada : '0';

        $sql = "SELECT mtpvalor FROM projovemurbano.metasdoprograma
                WHERE tpmid = 15 AND cmeid = {$rsCargaMeta['cmeid']} AND ppuid = {$_SESSION['projovemurbano']['ppuid']} AND suaid = {$rsSugestaoMeta['suaid']}";
        $metaPublicoUnidadesGeralAjustada = $db->pegaUm($sql);
        $metaPublicoUnidadesGeralAjustada = $metaPublicoUnidadesGeralAjustada ? $metaPublicoUnidadesGeralAjustada : '0';
        
    }

?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
    
    //Faz a verificação do perfil e desabilita o radio button
    jQuery(document).ready(function() {
        $('.mostra').click(function(){
			if( $(this).val() == 'TRUE' ){
				$('.tr_sugestao_ampliacao').show();
			}else{
				$('.tr_sugestao_ampliacao').hide();
			}
        });
        $('input[name*="mtpvalor"][type="text"]').blur(function(){
			var total = 0;
			var atual;
			$('input[name*="mtpvalor"][type="text"]').each(function(){
				atual = $(this).val();
				if( atual == '' ){
					atual = 0;
				}
				total = parseFloat(total) + parseFloat( atual );
			});
			$('#mtpvalor_total').val(total);
			$('#div_total_ajustado').html('<b>'+total+'</b>');
        });
        $('.salvar').click(function(){
            if( $('#suaverdade').attr('checked') ){
                var erro = false;
                $('.obrigatorio').each(function(){
                    if( $(this).val() == '' ){
                        $(this).focus();
                        erro = true;
                        return false;
                    }
                });
                if( erro ){
                    alert('Campo obrigatório.');
                    return false;
                }
                
            }
			$('#req').val('salvar');
			$('#form').submit();			
        });
    });
</script> 
<form id="form" name="form" method="POST">
    <input type="hidden" name="req" id="req" value="">
    <input type="hidden" name="cmeid" value="<?=$rsCargaMeta['cmeid'] ?>"/>
    <input type="hidden" name="suaid" value="<?= $rsSugestaoMeta['suaid']; ?>">
    <input type="hidden" name="estuf" value="<?= $_SESSION['projovemurbano']['estuf']; ?>">
    <input type="hidden" name="muncod" value="<?= $_SESSION['projovemurbano']['muncod']; ?>">
    <input type="hidden" name="ppuid" id="ppuid" value="<?= $_SESSION['projovemurbano']['ppuid']; ?>">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
        <tr>
            <td class="SubTituloDireita" width="30%">Orientações:</td>
            <td>A sugestão de meta é importante para a análise pela SECADI, possibilitando ou não a ampliação da meta original.</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Sugestão de ampliação de meta:</td>
            <td>
                <input type="radio" class="mostra" <?=$habilitaMetaSugerida == 'N' ? 'disabled="disabled"' : ''; ?> name="suaverdade" id="suaverdade" value="TRUE" <?= ($suaverdade == "t" && $habilitaMetaSugerida == 'S' ? "checked" : ""); ?> > Sim 
                <input type="radio" class="mostra" <?=$habilitaMetaSugerida == 'N' ? 'disabled="disabled"' : ''; ?> name="suaverdade" value="FALSE" <?= (($suaverdade == "f" || $habilitaMetaSugerida == 'N') || ($suaverdade != "t" || $habilitaMetaSugerida != 'S') ? "checked" : ""); ?> > Não
                <?=$habilitaMetaSugerida == 'N' ? '<input type="hidden" name="suaverdade" value="FALSE" />' : ''; ?> 
            </td>
        </tr>
        <tr class="tr_sugestao_ampliacao" <?= (($suaid && $suaverdade == "t" && $habilitaMetaSugerida == 'S' ) ? "" : "style=\"display:none\"") ?>>
			<td class="SubTituloDireita">Meta total sugerida:</td>
			<td>
				<b><?=$rsCargaMeta['cmemeta'] ?></b> Estudantes
			</td>
		</tr>
        <tr class="tr_sugestao_ampliacao" <?= (($suaid && $suaverdade == "t" && $habilitaMetaSugerida == 'S' ) ? "" : "style=\"display:none\"") ?>>
			<td class="SubTituloDireita">Meta total ajustada:</td>
			<td>
				<div id="div_total_ajustado" style="float:left"><b><?=$metaAjustada ?></b></div><div style="float:left">&nbsp;Estudantes</div>
				<input type="hidden" name="mtpvalor[3]" id="mtpvalor_total" value="<?=$metaAjustada ?>" />
			</td>
		</tr>
		
        <tr class="tr_sugestao_ampliacao" <?= (($suaid && $suaverdade== "t" && $habilitaMetaSugerida == 'S' ) ? "" : "style=\"display:none\"") ?>>
			<td class="SubTituloDireita">Meta Publico Juventude Viva sugerida:</td>
			<td>
				<b>
					<input type="hidden" name="mtpvalor[8]" value="<?=$rsCargaMeta['juventude']?>"/>
					<?=$rsCargaMeta['juventude']?>
				</b> 
					Estudantes
			</td>
		</tr>
        <tr class="tr_sugestao_ampliacao" <?= (($suaid && $suaverdade == "t" && $habilitaMetaSugerida == 'S' ) ? "" : "style=\"display:none\"") ?>>
			<td class="SubTituloDireita">Meta Publico Juventude Viva ajustada:</td>
			<td>
				<?=campo_texto('mtpvalor[9]', 'S', $habilita, 'Meta ajustada', 10, 8, "########", "", '', '', 0, 'id="mtpvalor[9]"', '', $metaPublicoJuventudeVivaAjustada); ?> 
			</td>
		</tr>
		
        <tr class="tr_sugestao_ampliacao" <?= (($suaid && $suaverdade == "t" && $habilitaMetaSugerida == 'S' ) ? "" : "style=\"display:none\"") ?>>
			<td class="SubTituloDireita">Meta Público Unidades Prisionais sugerida:</td>
			<td>
				<b>
					<input type="hidden" name="mtpvalor[11]" value="<?=$rsCargaMeta['prisional']?>"/>
					<?=$rsCargaMeta['prisional']?>
				</b> 
					Estudantes
			</td>
		</tr>
        <tr class="tr_sugestao_ampliacao" <?= (($suaid && $suaverdade == "t" && $habilitaMetaSugerida == 'S' ) ? "" : "style=\"display:none\"") ?>>
			<td class="SubTituloDireita">Meta Público Unidades Prisionais ajustada:</td>
			<td>
				<?=campo_texto('mtpvalor[12]', 'S', $habilita, 'Meta ajustada', 10, 8, "########", "", '', '', 0, 'id="mtpvalor[12]"', '', $metaPublicoUnidadesPrisionaisAjustada); ?> 
			</td>
		</tr>
		
        <tr class="tr_sugestao_ampliacao" <?= (($suaid && $suaverdade == "t" && $habilitaMetaSugerida == 'S' ) ? "" : "style=\"display:none\"") ?>>
			<td class="SubTituloDireita">Meta Público Geral sugerida:</td>
			<td>
				<b>
					<input type="hidden" name="mtpvalor[14]" value="<?=$rsCargaMeta['geral']?>"/>
					<?=$rsCargaMeta['geral']?>
				</b> 
					Estudantes
			</td>
		</tr>
        <tr class="tr_sugestao_ampliacao" <?= (($suaid && $suaverdade == "t" && $habilitaMetaSugerida == 'S' ) ? "" : "style=\"display:none\"") ?>>
			<td class="SubTituloDireita">Meta Público Geral ajustada:</td>
			<td>
				<?=campo_texto('mtpvalor[15]', 'S', $habilita, 'Meta ajustada', 10, 8, "########", "", '', '', 0, 'id="mtpvalor[15]"', '', $metaPublicoUnidadesGeralAjustada); ?> 
			</td>
		</tr>
		<?if($habilita == 'S'){?>
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" class="salvar" value="Salvar">
            </td>
        </tr>
        <?}?>
    </table>
</form>