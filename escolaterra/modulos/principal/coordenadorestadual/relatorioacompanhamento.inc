<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>
function gravarRelatorioAcompanhamento() {

	var raerealizouacaoescolaterra_r = jQuery("[name^='raerealizouacaoescolaterra']:enabled:checked").length;

	if(raerealizouacaoescolaterra_r==0) {
		alert('Marque se realizou ação para a Escola da Terra esse mês');
		return false;
	}	

	var raetempoformacao_r = jQuery("[name^='raetempoformacao']:enabled:checked").length;

	if(raetempoformacao_r==0) {
		alert('Marque em que tempo de formação');
		return false;
	}

	if(jQuery('#raeprincipaisatividades').val()=='') {
		alert('Preencha  as principais atividades realizadas');
		return false;
	}

	var raeparticipouencontro_r = jQuery("[name^='raeparticipouencontro']:enabled:checked").length;

	if(raeparticipouencontro_r==0) {
		alert('Marque se participou de encontro presencial na/com a universidade');
		return false;
	}

	var raeparticipouencontro_r_sim = jQuery("[name^='raeparticipouencontro'][value='S']:enabled:checked").length;

	if(raeparticipouencontro_r_sim==1) {
		
		if(jQuery('#raedataencontro').val()=='') {
			alert('Preencha a data do encontro presencial');
			return false;
		}

		var raeparticipacaoencontro_r = jQuery("[name^='raeparticipacaoencontro']:enabled:checked").length;

		if(raeparticipacaoencontro_r==0) {
			alert('Marque quem participou desse encontro presencial');
			return false;
		}

		if(jQuery('#raeatividadesocorridasencontro').val()=='') {
			alert('Preencha as atividades ocorridas nesse encontro presencial');
			return false;
		}

		var raesatisfacaoencontro_r = jQuery("[name^='raesatisfacaoencontro']:enabled:checked").length;

		if(raesatisfacaoencontro_r==0) {
			alert('Marque o grau de satisfação/entusiasmo dos tutores e/ou cursistas nesse encontro');
			return false;
		}

		if(jQuery('#raedestaquespositivosencontro').val()=='') {
			alert('Preencha os pontos positivos desse encontro presencial');
			return false;
		}

		if(jQuery('#raedestaquesnegativosencontro').val()=='') {
			alert('Preencha os pontos negativos desse encontro presencial');
			return false;
		}

		if(jQuery('#raeaperfeicoarencontro').val()=='') {
			alert('Preencha os pontos para aperfeiçoar o evento');
			return false;
		}
		
	}
	
	
	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "iusid");
	input.setAttribute("value", "<?=$_SESSION['escolaterra']['coordenadorestadual']['iusid'] ?>");
	document.getElementById("formulario_relatorioacompanhamento").appendChild(input);

	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "fpbid");
	input.setAttribute("value", "<?=$_REQUEST['fpbid'] ?>");
	document.getElementById("formulario_relatorioacompanhamento").appendChild(input);
	
	
	document.getElementById("formulario_relatorioacompanhamento").submit();

	
}

function participouEncontro(obj) {
	
	if(obj.value=='S') {
		jQuery('#td_encontropresencial').css('display','');
	} else {
		jQuery('#td_encontropresencial').css('display','none');
	}
}

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Principal</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/escolaterra/escolaterra.php?modulo=principal/coordenadorestadual/coordenadorestadualexecucao&acao=A&aba=relatorioacompanhamento"); ?></td>
</tr>
<tr>
	<td colspan="2" class="SubTituloCentro">
	<?
	carregarPeriodoReferencia(array('ufpid'=>$_SESSION['escolaterra']['coordenadorestadual']['ufpid'],'fpbid'=>$_REQUEST['fpbid']));
	?>
	</td>
</tr>
<? if($_REQUEST['fpbid']) : ?>


<?

$sql = "SELECT * FROM escolaterra.relatorioacompanhamentocoordenadorestadual WHERE iusid='".$_SESSION['escolaterra']['coordenadorestadual']['iusid']."' AND fpbid='".$_REQUEST['fpbid']."'";
$relatorioacompanhamento = $db->pegaLinha($sql);

if(!$relatorioacompanhamento) {

	$docid = wf_cadastrarDocumento( TPD_RELATORIOCE, 'RELATÓRIO CE : '.$_SESSION['escolaterra']['coordenadorestadual']['descricao'].' FPBID: '.$_REQUEST['fpbid'].' IUSD: '.$_SESSION['escolaterra']['coordenadorestadual']['iusid'] );
	
	$relatorioacompanhamento['docid'] = $docid;

	$sql = "INSERT INTO escolaterra.relatorioacompanhamentocoordenadorestadual(
            iusid, docid, fpbid)
    		VALUES ('".$_SESSION['escolaterra']['coordenadorestadual']['iusid']."', {$docid}, '".$_REQUEST['fpbid']."');";
	
	$db->executar($sql);
	$db->commit();
	
} else {
	extract($relatorioacompanhamento);
}




?>
<tr>
	<td colspan="2">
	<form method="post" name="formulario_relatorioacompanhamento" id="formulario_relatorioacompanhamento">
	<input type="hidden" name="requisicao" value="gravarRelatorioAcompanhamentoCoordenadorEstadual">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td colspan="3" class="SubTituloCentro">Descrição das Atividades Realizadas de Acompanhamento Pedagógico</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="30%">1. Você realizou ação para a Escola da Terra esse mês?</td>
		<td><input type="radio" name="raerealizouacaoescolaterra" value="S" <?=(($raerealizouacaoescolaterra=='S')?'checked':'') ?>> Sim <input type="radio" name="raerealizouacaoescolaterra" value="N" <?=(($raerealizouacaoescolaterra=='N')?'checked':'') ?>> Não</td>
		<td rowspan="3" valign="top" width="5%">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $relatorioacompanhamento['docid'], array('fpbid' => $_REQUEST['fpbid'], 'iusid' => $_SESSION['escolaterra']['coordenadorestadual']['iusid']) );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="30%">2. Em que tempo de formação</td>
		<td><input type="radio" name="raetempoformacao" value="U" <?=(($raetempoformacao=='U')?'checked':'') ?>> Universidade <input type="radio" name="raetempoformacao" value="C" <?=(($raetempoformacao=='C')?'checked':'') ?>> Comunidade <input type="radio" name="raetempoformacao" value="A" <?=(($raetempoformacao=='A')?'checked':'') ?>> Ambos</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="30%">3. Descreva, sinteticamente,  as principais atividades realizadas</td>
		<td><? echo campo_textarea( 'raeprincipaisatividades', 'S', 'S', '', '70', '4', '2000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="30%">4. Participou de encontro presencial na/com a Universidade?</td>
		<td><input type="radio" name="raeparticipouencontro" value="S" onclick="participouEncontro(this);" <?=(($raeparticipouencontro=='S')?'checked':'') ?>> Sim <input type="radio" name="raeparticipouencontro" value="N" onclick="participouEncontro(this);" <?=(($raeparticipouencontro=='N')?'checked':'') ?>> Não</td>
	</tr>
	
	<tr id="td_encontropresencial" <?=(($raeparticipouencontro=='S')?'':'style="display:none;"') ?>>
		<td colspan="2">
		<br>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		
		<tr>
			<td class="SubTituloDireita" width="30%">Em que data ocorreu o encontro presencial?</td>
			<td><?=campo_data2('raedataencontro','S', 'S', 'Data encontro presencial', 'S', '', '', '', '', '', 'raedataencontro'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Nesse encontro presencial teve participação de:</td>
			<td><input type="radio" name="raeparticipacaoencontro" value="T" <?=(($raeparticipacaoencontro=='T')?'checked':'') ?>> Tutores <input type="radio" name="raeparticipacaoencontro" value="C" <?=(($raeparticipacaoencontro=='C')?'checked':'') ?>> Cursistas <input type="radio" name="raeparticipacaoencontro" value="A" <?=(($raeparticipacaoencontro=='A')?'checked':'') ?>> Ambos</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Descreva, resumidamente, algumas das atividades ocorridas nesse encontro presencial</td>
			<td><? echo campo_textarea( 'raeatividadesocorridasencontro', 'S', 'S', '', '70', '4', '2000'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">De modo geral,  como você avalia  a satisfação/entusiasmo  dos tutores e/ou cursistas  nesse encontro presencial ?</td>
			<td><input type="radio" name="raesatisfacaoencontro" value="P" <?=(($raesatisfacaoencontro=='P')?'checked':'') ?>> Pouco satisfeitos <input type="radio" name="raesatisfacaoencontro" value="S" <?=(($raesatisfacaoencontro=='S')?'checked':'') ?>> Satisfeitos <input type="radio" name="raesatisfacaoencontro" value="M" <?=(($raesatisfacaoencontro=='M')?'checked':'') ?>> Muito satisfeitos</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Quais destaques positivos  do referido do encontro presencial?</td>
			<td><? echo campo_textarea( 'raedestaquespositivosencontro', 'S', 'S', '', '70', '4', '2000'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Quais destaques negativos - inibidores?</td>
			<td><? echo campo_textarea( 'raedestaquesnegativosencontro', 'S', 'S', '', '70', '4', '2000'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">O que você sugere para aperfeiçoar o evento?</td>
			<td><? echo campo_textarea( 'raeaperfeicoarencontro', 'S', 'S', '', '70', '4', '2000'); ?></td>
		</tr>
		
		
		</table>
		
		</td>
	</tr>
	
	</table>
	
	</form>
	</td>
</tr>


<? endif; ?>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td><input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_pro ?>';"> <input type="button" value="Gravar" onclick="gravarRelatorioAcompanhamento();"></td>
</tr>
</table>