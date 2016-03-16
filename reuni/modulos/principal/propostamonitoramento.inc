<?php
// monta cabeçalho

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'reuni/www/funcoes.php';

print '<br/>';

print '<br/>';


$unidade = $db->usuarioUnidadesPermitidas('reuni');

$menu[0] = array("descricao" => "Lista de Unidades", "link"=> "/reuni/reuni.php?modulo=principal/listamonitoramento&acao=C");
$menu[1] = array("descricao" => "Monitoramento Acadêmico", "link"=> "/reuni/reuni.php?modulo=principal/cadastromonitoramento&acao=C&unicod=".base64_decode($_REQUEST['unicod'])."");
$menu[2] = array("descricao" => "Item Monitoramento", "link"=> "/reuni/reuni.php?modulo=principal/propostamonitoramento&acao=C&unicod=".$_REQUEST['unicod']."&id=".$_REQUEST['id']."");

echo montarAbasArray($menu, "/reuni/reuni.php?modulo=principal/propostamonitoramento&acao=C&unicod=".$_REQUEST['unicod']."&id=".$_REQUEST['id']."");

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Monitoramento Acadêmico', '&nbsp;' );


$habilitado = verificaPerfilConsulta();
$habilitado = $habilitado ? 'N' : 'S'; 

//$habilitado = 'S';

$unicod = base64_decode($_REQUEST['unicod']);
$partes = explode('_',md5_decrypt($_REQUEST['id'],''));

$codPergunta = $partes[0];
$pai 	  	 = $partes[1];
$filho 		 = $partes[2];


$faseSesu 		= reuni_podeEditarParecerRespostaSesu( $codPergunta );
$faseAdhoc 		= reuni_podeEditarParecerRespostaAdhoc( $codPergunta );
$faseComissao 	= reuni_podeEditarParecerRespostaComissao( $codPergunta );

$unicod 		= (int)base64_decode($_REQUEST['unicod']);

$sql = "
		select rp.rspdsc -- Metas cadastradas
		, si.sitdsc -- Qualificação
		, pa.pardsc -- Parecer ADHOC
		from reuni.pergunta pr
		left join reuni.resposta rp on pr.prgcod = rp.prgcod
		left join reuni.parecer pa on rp.rspcod = pa.rspcod
		left join reuni.situacao si on pa.sitcod = si.sitcod
		where rp.unicod = '".$unicod."' and rp.unitpocod = 'U' -- Fundação Universidade de Brasília - variável
		and pr.grpcod = ".$filho." -- A.1 - variável
		and pr.prgdsc = 'Metas a serem alcançadas com o cronograma de execução'
		and pa.tpacod = 2 -- Parecer ADHOC -- fixo
";
$dados_tela	= $db->pegaLinha($sql);

$situacao 		= $dados_tela['sitdsc'];
$parecerAdhoc 	= $dados_tela['pardsc'];
$resposta 		= $dados_tela['rspdsc'];

$sql_unidade 	= "select unidsc from public.unidade where unicod = '".base64_decode($_REQUEST["unicod"])."'";
$unidade 		= $db->pegaUm($sql_unidade);

?>

<!-- corpo da página -->

<?php

function nomePaiFilho($cod,$p=false){
	global $db;
	if (!$cod || trim($cod) == '')
        return;

	$sql = "select grpdsc from reuni.grupopergunta where grpcod = $cod";
	$dado = $db->pegaUm($sql);

	if($p){
		$item = 'Item:';
	}else{
		$item = 'Tópico:';
	}

	if($dado){
		$saida = '<tr><td class="SubTituloDireita" align="right"><b>' . $item . '</b></td><td>' .$dado . '</td></tr>';
		return $saida;
	}
}

function nomePergunta($cod,$pai){
	global $db;
	$sql = "select prgdsc from reuni.pergunta where prgcod = $cod";
	$dado = $db->pegaUm($sql);
	if($dado)
	{
		if(	$pai > 28 )		
		{
			$saida = '<tr><td class="SubTituloDireita" align="right"><b>Item:</b></td><td>' .$dado . '</td></tr>';
		}
		return $saida;
	}
}

function insertUpload($unicod, $filho){
	global  $db;
	$sql = "select count(*) from reuni.monitoramento where unicod = '$unicod' and grpcod = $filho";
	$dado = $db->pegaUm($sql);

	if($dado > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

function montaSituacao($nome='',$readOnly=false,$codParecer=''){
	global  $db;
	global $habilitado;
	$sql = "select sitdsc , sitcod from reuni.situacao";
	$dado = $db->carregar($sql);
	$saida = '';
	$check = '';

	foreach ($dado as $row)
	{
		if($row['sitcod'] == $codParecer) $check = "checked='checked'";
		if($habilitado == 'N') $desabilitado = "disabled='disabled'";		
		$saida .= "<input onclick='atualizarQualificacao()' type='radio' name='sitcod' $desabilitado $check value='".$row['sitcod']."'>". $row['sitdsc']	. "&nbsp;";
		$check = '';
	}
	return $saida;
}


if((isset($_REQUEST['enviar'])) && ($_REQUEST['enviar']!=""))
{	
	
	
	if ( $_REQUEST['staid'] != $atividade['staid'] && $_REQUEST['staid'] == STATUS_CONCLUIDO ) 
	{
		$_REQUEST['atidataconclusao'] = $_REQUEST['atidataconclusao'] ? $_REQUEST['atidataconclusao'] : date( 'Y-m-d' );
	}
		
	$unicod 			= base64_decode($_REQUEST['unicod']);	
	$unitpocod 			= 'U';
	$sitcod 			= $_REQUEST['sitcod'];
	$staid 				= $_REQUEST['staid'];
	$metas 				= $_REQUEST['metas'];
	$novoparecer 		= $_REQUEST['novoparecer'];
	$providencia 		= $_REQUEST['providencia'];
	$atiporcentoexec 	= $_REQUEST['atiporcentoexec'];
	$corcod 			= $_REQUEST['acompanhamento'];
	
	if(insertUpload($unicod, $filho))
	{
		$sql = "update reuni.monitoramento 
			set  			
				unicod = $unicod, 
				unitpocod = '$unitpocod',
				grpcod = $filho,
				sitcod = $sitcod,
				staid = $staid,	 
				monparecer = '$novoparecer',
				monprovidencias = '$providencia',
				monpercentual = $atiporcentoexec, 
				mondata = now(),
				usucpf = '".$_SESSION['usucpf']."', 
				corcod = ".$corcod."
			where grpcod= $filho and unicod = '$unicod' and unitpocod = '$unitpocod'";	
	}
	else
	{
		$sql = "insert into reuni.monitoramento 
						(unicod, unitpocod, grpcod, sitcod, staid, monparecer, monprovidencias, monpercentual, mondata, usucpf, corcod) 
				values ('$unicod','$unitpocod', $filho, $sitcod, $staid, '$novoparecer', '$providencia', '$atiporcentoexec', now(), '".$_SESSION['usucpf']."', ".$corcod." )";
	}
	$db->executar($sql);
	$db->commit();
	$db->sucesso('principal/propostamonitoramento','&unicod='.$_REQUEST['unicod'].'&id='.$_REQUEST['id']);

}else{
	$sql = "select unicod, unitpocod, grpcod, sitcod, staid, monparecer, monprovidencias, monpercentual, corcod from reuni.monitoramento where unicod = '$unicod' and grpcod = $filho";
	
	$dado = $db->pegaLinha($sql);
	
	if($dado){
		$unitpocod = 'U';
		$sitcod = $dado['sitcod'];
		$staid = $dado['staid'];		
		$novoparecer = $dado['monparecer'];
		$providencia = $dado['monprovidencias'];	
		$atiporcentoexec = $dado['monpercentual'];
		$corcod = $dado['corcod'];
	}	
}	

?>

<form action="" method="post" name="formulario" enctype="multipart/form-data">
<input type="hidden" id="enviar" name="enviar" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" align="right"><b>Unidade:</b></td>
		<td><?=$unidade; ?>
		</td>
	</tr>
	<?= nomePaiFilho($pai);?>
	<?= nomePaiFilho($filho,1); ?>	
	
	<tr>
		<td class="SubTituloDireita" align="right"><b>Metas:</b></td>
		
		<td><div style="float: left; width: 649px; height: 150px; overflow: auto; text-align: justify; border: 1px solid; color: #888888;" >
			<?=$resposta; ?>
		</div>	
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			<?=$situacao;?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right"><b>Parecer ADHOC:</b>
		</td>		
		<td><div style="float: left; width: 649px; height: 150px; overflow: auto; text-align: justify; border: 1px solid; color: #888888;" >
			<?=$parecerAdhoc; ?>
		</div>	
				
		</td>	
		<tr>
			<td bgcolor="#c0c0c0" colspan="2" align="center"><b>Dados Monitoramento<b>
			</td>
		</tr>			
	<tr>
		<td class="SubTituloDireita" align="right">
			<b>Situação:</b>
		</td>		
		<td>
		<?php
			$sql = "select s.staid as codigo, s.stadsc as descricao from reuni.status s";
			$db->monta_combo( "staid", $sql, $habilitado, '', 'selecionarSituacao', '' );
			echo(obrigatorio());
		?>
		</td>	
		
		<?php 
			 print '<tr><td align="right" class="subtitulodireita"><b>Acompanhamento da Situação:</b></td><td>';
			  $sql = "select corcod as CODIGO,corsignificado as desc , corimgav as imagem from cor where corstatus='A' order by corcod ";
			  $RSradio = $db->record_set($sql);
		      $nlinhasradio = $db->conta_linhas($RSradio);
		      for ($j=0; $j<=$nlinhasradio;$j++)
		      {
		      	if($habilitado == 'N') $desabilitado = "disabled='disabled'";
		        $res2 =  $db->carrega_registro($RSradio,$j);
		        if(is_array($res2)) foreach($res2 as $k=>$v) ${$k}=$v;
		        if ($codigo == $corcod) $ck = ' checked '; else $ck ='';
		        print "<input type='radio' value=".$codigo.$ck." name='acompanhamento'  title='".$desc."' $desabilitado>";
		        print "<img src='../imagens/".trim($imagem)."' title='".$desc."'>";
	          }
	          echo(obrigatorio());
	          print "</td></tr>";
		?>
		
	</tr>	
		<tr>
		<td align='right' class="SubTituloDireita"><b>Andamento:</b></td>
		<td>
			<select name="atiporcentoexec" onchange="selecionarAndamento( this.value );" class="CampoEstilo" <? if($habilitado == 'N') echo("disabled='disabled'"); ?>>
				<?php for( $porcentagem = 0; $porcentagem <= 100; $porcentagem += 10 ): ?>
					<option <?= $atiporcentoexec == $porcentagem ? 'selected' : '' ?> value="<?= $porcentagem ?>"><?= $porcentagem ?>%</option>
				<?php endfor; ?>
			</select>
			<?=obrigatorio(); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right"><b>Parecer ADHOC:</b>
		</td>
		<td>
			<?php				
				echo campo_textarea( 'novoparecer', 'S', $habilitado, '',130, 10,2000);			
			?>
			
		</td>	
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			<?=montaSituacao('Adhoc',$faseAdhoc, $sitcod)?>
			<?=obrigatorio(); ?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right"><b>Providência:</b>
		</td>
		<td>
			<?php				
				echo campo_textarea( 'providencia', 'N', $habilitado, '',130, 10,2000);				
			?>			
		</td>
		
	</tr>

	<tr bgcolor="#cococo">
		<td align="right"></td>

		<td align="left">
		
		<div style="float:left">
		<!--  <input class="botao" type="button" value="Voltar" onclick="voltar();"> -->	
		<?php 
			if($habilitado != 'N') echo(" <input class='botao' type='button' value='Salvar' onclick='salvar();'>	");
		?>			
		</div>	
		</td>
	</tr>
	
</table>
</form>


<script>

	<?
		if($sitcod == 1){
		?>
			desabilitar();
		<?
		}
	?>

	function selecionarAndamento( valor ){
		if ( document.formulario.staid.value == '3' || document.formulario.staid.value == '4' ) {
			return;
		}
		switch ( valor ) {
			case '0':
				document.formulario.staid.value = '1';
				break;
			case '100':
				document.formulario.staid.value = '5';
				break;
			default:
				document.formulario.staid.value = '2';
				break;
		}
	}
	
	function selecionarSituacao( valor ){
	
		//if ( document.formulario.atiporcentoexec.value > 0 && document.formulario.atiporcentoexec.value < 100 ) {
		//	return;
		//}
		habilitar();
		switch ( valor ) {
			case '1':
				document.formulario.atiporcentoexec.value = '0';
				break;
			case '2':
				document.formulario.atiporcentoexec.value = '10';
				break;
			case '5':
				document.formulario.atiporcentoexec.value = '100';
				break;
			case '6':			
				desabilitar();
				break;	
			default:								
				break;
		}
	}
	
	function desabilitar(){
	
		document.formulario.atiporcentoexec.value = '0';			
		document.formulario.atiporcentoexec.disabled = 'disabled';
		
		document.formulario.novoparecer.value = 'Não se aplica.';
		var novoparecer = document.getElementsByName('novoparecer')[0];
		novoparecer.readOnly = true;	

		//desabilitando qualificação
		var max = document.formulario.sitcod.length;	
		document.formulario.sitcod[max - 1].checked = 'true';		
		for(i=0; i< max; i++ ){
			document.formulario.sitcod[i].disabled = true;
		}	
		//desbilitando acompanhamento
		var max_acompanhamento = document.formulario.acompanhamento.length;	
		document.formulario.acompanhamento[0].checked = 'true';	
		for(i=0; i< max_acompanhamento; i++ ){
			document.formulario.acompanhamento[i].disabled = true;
		}
	}

	function atualizarQualificacao(){
		var sitcod = document.getElementsByName('sitcod');
		if(pegaRadio(sitcod)  == 1) {
			document.formulario.staid.options[5].selected = true;
			desabilitar();
		}			
	}
	
	function habilitar(param){	
					
		document.formulario.atiporcentoexec.disabled = false;		
		
		var novoparecer = document.getElementsByName('novoparecer')[0];
		novoparecer.readOnly = false;	
		
		var max = document.formulario.sitcod.length;
		var max_acompanhamento = document.formulario.acompanhamento.length;	
		if(param != 1){			
			if(document.formulario.sitcod[0].disabled){			
				for(i=0; i< max; i++ ){
					document.formulario.sitcod[i].checked = false;
				}	
			}	

			if(document.formulario.acompanhamento[0].disabled){			
				for(i=0; i< max_acompanhamento; i++ ){
					document.formulario.acompanhamento[i].checked = false;
				}	
			}	
		}		
		
		for(i=0; i< max; i++ ){
			document.formulario.sitcod[i].disabled = false;
		}

		for(i=0; i< max_acompanhamento; i++ ){
			document.formulario.acompanhamento[i].disabled = false;
		}	
		
	}

	function pegaRadio(objRadio){
		for ( var i = 0 ; i < objRadio.length ; i++ ){
			if (objRadio[i].checked) {
				return objRadio[ i ].value;
				break ;
			}
		}
		return -1;
	}
	
	function salvar(){
		habilitar(1);
		var msg = '';			
		if( pegaRadio(document.formulario.sitcod) == -1){
			msg += 'O campo Qualificação é de preenchimento obrigatório';
		}

		if( pegaRadio(document.formulario.acompanhamento) == -1){
			msg += 'O campo Acompanhamento da Situação é de preenchimento obrigatório';
		}	
		if(document.formulario.atiporcentoexec.value == ''){
			msg += 'O campo Andamento é de preenchimento obrigatório';
		}		
		if(document.getElementById('novoparecer').value == ''){
			msg += 'O campo Parecer ADHOC é de preenchimento obrigatório';
		}		
		if(msg != ''){
			alert(msg);
			return;
		}else{				
			document.formulario.enviar.value = 1;
			document.formulario.submit();			
		}		
		
	} 
	
	//function voltar(){
	//	window.location.href = 'reuni.php?modulo=principal/cadastromonitoramento&acao=C&unicod=<?=$unicod ?>';
	//}
</script>