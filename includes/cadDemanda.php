<?php

function EnviarArquivo($arquivo,$dados=null,$dmdid){
	global $db;

	if (!$arquivo || !$dmdid)
		return false;
		
	// obtém o arquivo
	#$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	
			(
				arqnome,
				arqextensao,
				arqdescricao,
				arqtipo,
				arqtamanho,
				arqdata,
				arqhora,
				usucpf,
				sisid
			)VALUES(
				'".current(explode(".", $arquivo["name"]))."',
				'".end(explode(".", $arquivo["name"]))."',
				'".$dados["arqdescricao"]."',
				'".$arquivo["type"]."',
				'".$arquivo["size"]."',
				'".date('Y-m-d')."',
				'".date('H:i:s')."',
				'".$_SESSION["usucpf"]."',
				". $_SESSION["sisid"] ."
			) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	//Insere o registro na tabela demandas.anexos
	$sql = "INSERT INTO demandas.anexos 
			(
				dmdid,
				arqid,
				anxdtinclusao,
				anxstatus
			)VALUES(
			    ". $dmdid .",
				". $arqid .",
				now(),
				'A'
			);";
	$db->executar($sql);
	
	if($_SESSION['sisid'] == 23){
		$caminhoPasta = APPRAIZ . 'arquivos/demandas/'. floor($arqid/1000);
	} else {
		$caminhoPasta = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000);
	}
    # Fix corrigindo tipo de barras.
    $caminhoPasta = str_replace(' ', '', str_replace('/', "\ ", $caminhoPasta));
    # Fix criando estrutura de pastas caso não exista.
	if(!is_dir($caminhoPasta)) {
		mkdir($caminhoPasta, 0777, TRUE);
	}
    # Fix caminho completo do arquivo.
    $caminho = $caminhoPasta. str_replace(' ', '', '\ '). $arqid;

	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			$original_y = $height;
			// se a largura for maior que altura
			if($original_x > $original_y) {
  	 			$porcentagem = (100 * 640) / $original_x;      
			}else {
   				$porcentagem = (100 * 480) / $original_y;  
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				return false;
			}
	}
	
	
	$db->commit();
	return true;

}

function salvaDemanda(){
	global $db;
	
	$select = array();
	$dado	= array();
	
	if ($_POST['outrouser'] == 1){
		$select[0] = 'dmdnomedemandante';
		$dado[0]   = $_POST['usudemandante'];
		$select[1] = "dmdemaildemandante";
		$dado[1]   = $_POST['usermail'];	
		$select[2] = "usucpfdemandante";
		if($_POST['usucpf']){
			$dado[2]   = $_POST['usucpf'];
			$usucpf = $_POST['usucpf'];
		}else{
			$dado[2]   = $_SESSION['usucpf'];
			$usucpf = $_SESSION['usucpf'];
		}	
	}else{
		$select[0] = "usucpfdemandante";
		$dado[0]   = $_POST['usucpf'];	
		$usucpf = $_POST['usucpf'];
	}

	
	if($_POST['horarioA'] && $_POST['horarioN']){
		$horario = "T";	
	}elseif($_POST['horarioA'] && !$_POST['horarioN']){
		$horario = "A";
	}elseif(!$_POST['horarioA'] && $_POST['horarioN']){
		$horario = "N";
	}else{
		$horario = "C";
	}
	
	if(!$_POST['atendimentoRemoto']) $_POST['atendimentoRemoto'] = 'f';
	if(!$_POST['dmdatendurgente']) $_POST['dmdatendurgente'] = 'f';
	
	if(!$_POST['dmdqtde']) $_POST['dmdqtde'] = '1';
	
	//Pega o analista do sistema!
	$sql = "SELECT usucpf FROM demandas.usuarioresponsabilidade WHERE pflcod = 237 AND rpustatus = 'A' AND sidid = ".$_SESSION['sidid'];
	$usucpfexecutor = $db->pegaUm( $sql );
	$sql = sprintf("INSERT INTO demandas.demanda
					(
						%s
						usucpfinclusao,
						tipid,
						sidid,
						dmdtitulo,
						dmddsc,
						dmdreproducao,
						dmdstatus,
						laaid,
						dmdsalaatendimento,
						unaid,
						dmdqtde,
						dmdhorarioatendimento,
						dmdatendremoto,
						dmddatainclusao,
						dmdatendurgente,
						dmddatainiprevatendimento,
						dmddatafimprevatendimento,
						priid,
						dmdclassificacao,
						dmdclassificacaosistema,
						usucpfexecutor,
                                                celid
					)VALUES(
						%s
						'%s',
						%s,
						%s,
						'%s',
						'%s',
						'%s',
						'A',
						 %s,
						'%s',
						%d,
						%d,
						'%s',
						'%s',
						'%s',
						'%s',
						'%s',
						'%s',
						%s,
						'%s',
						'%s',
						'%s',
						'%s'
					) RETURNING dmdid ",
					(count($dado) > 0 ? implode(',',$select)."," : ''),
					(count($dado) > 0 ? "'".implode("','",$dado)."'," : ''), 
					$_SESSION['usucpf'],
					$_POST['tipid'] ? $_POST['tipid'] : 'null',
					($_SESSION['sidid'] ? $_SESSION['sidid'] : 'null'),
					$_POST['assunto'],
					$_POST['necessidade'],
					$_POST['reproducao'],
					($_POST['laaid'] ? $_POST['laaid'] : 'null'),
					$_POST['dmdsalaatendimento'],
					$_POST['unaid'],
					$_POST['dmdqtde'],
					$horario,
					$_POST['atendimentoRemoto'],
					date('Y-m-d H:i:s'),
					$_POST['dmdatendurgente'],
					date('Y-m-d H:i:s'),
					date('Y-m-d H:i:s'),
					$_POST['priid'] ? $_POST['priid'] : 1,
					$_POST['dmdclassificacao'],
					$_POST['dmdclassificacaosistema'],
					$usucpfexecutor ? $usucpfexecutor : 'null',
                                        $_POST['celid']
                
					);
        
	$dmdid = $db->pegaUm($sql);
	
	$db->commit();
	
	return $dmdid;
}


/*
 * Executa a função que salva a demanda
 * Retorna mensagem de sucesso e redireciona a página
 */

if($_POST['varaux'] == 'okCad') {
 	 $dmdid 	= salvaDemanda();
 	 
	 if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], array('arqdescricao' => 'Arquivo de demanda do Monitoramento de Obras.'), $dmdid)) {
	 		$_SESSION['dmdid'] = $dmdid;
	 		
		 	?>
		 	<script>
		 		alert('Demanda cadastrada com sucesso!');
		 		location.href=window.location.href;
			</script>
			<?php
	 	die;
	 } else {
	 	die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");	
	 }
}

$usucpf = $_REQUEST['usucpf'] ? $_REQUEST['usucpf'] : $_SESSION['usucpf'];

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Solicitação de Demanda', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );

?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script>

d = document;

function validaForm()
{
	if (d.getElementsByName('assunto')[0].value == ''){
		d.getElementsByName('assunto')[0].focus();
		d.getElementsByName('assunto')[0].select();
		alert('O campo assunto, deve ser preenchido!');
		return false;
	}

	if (d.getElementsByName('necessidade')[0].value == ''){
		d.getElementsByName('necessidade')[0].focus();
		d.getElementsByName('necessidade')[0].select();
		alert('O campo necessidade, é obrigatório!');
		return false;
	}
	
	if (d.getElementById('dmdclassificacao').value == ''){
		alert('O campo classificação da demanda, é obrigatório!');
		return false;
	}
	
	if (d.getElementById('dmdclassificacaosistema').value == ''){
		alert('O campo tipo da demanda para sistemas de informação, é obrigatório!');
		return false;
	}
	
	d.getElementsByName('varaux')[0].value = 'okCad';
	return true;
	
}
</script>

<form id="formDemanda" action="" method="post" enctype="multipart/form-data" onsubmit="return validaForm();">
<input type="hidden" name="usucpf" id="usucpf" value="<?=$usucpf;?>">
<input type="hidden" name="outrouser" id="outrouser" value="0">
<input type="hidden" name="usermail" id="usermail" value="">
<input type="hidden" name="verificaqtd" id="verificaqtd" value="">
<input type="hidden" name="varaux" id="varaux" value="">
<input type="hidden" name="ordid" id="ordid" value="1">
<input type="hidden" name="celid" id="celid" value="2">
<input type="hidden" name="sisid" id="sisid" value="<?=$_SESSION['sisid'] ?>">
<input type="hidden" name="tipid" id="tipid" value="701">
<input type="hidden" name="atendimentoRemoto" value="f">
<input type="hidden" name="dmdatendurgente" value="f">
<input type="hidden" name="unaid" id="unaid" value="15">
<input type="hidden" name="lcaid" id="lcaid" value="3">
<input type="hidden" name="laaid" id="laaid" value="16">
<input type="hidden" name="dmdsalaatendimento" id="dmdsalaatendimento" value="43">
<input type="hidden" name="usdgabinete" id="usdgabinete" value="f">
<input type="hidden" name="usdramal" id="usdramal" value="9803">

<table id="tblFormDemanda" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align='right' class="SubTituloDireita" width="30%">Informe o <b>assunto</b> para a demanda:</td>
		<td><?= campo_texto( 'assunto', 'S', 'S', '', 80, 250, '', ''); ?></td>
	</tr>	
	<tr>
		<td align='right' class="SubTituloDireita"><b>Descreva</b> sua necessidade:</td>
		<td><?= campo_textarea('necessidade', 'S', 'S', '', 80, 5, 4000); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Descreva os passos para a reprodução do problema:<br><b>(Opcional)</b></td>
		<td><?= campo_textarea('reproducao', 'N ', 'S', '', 80, 5, 4000); ?></td>
	</tr>										
	<tr>
		<td align='right' class="SubTituloDireita">Anexe um arquivo:<br><b>(Opcional)</b></td>
		<td>
		 <input name="anexo" type="file" style="text-align: left; width: 83ex;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);" onmouseover="MouseOver(this);" class="normal" size="81">
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Classificação da demanda:</td>
		<td>
			<select name="dmdclassificacao" id="dmdclassificacao" >
				<option value=""> -- Selecione -- </option>
				<option value="I">Incidente</option>
				<option value="P">Resolução de problema</option>
				<option value="M">Requisição de mudança</option>
				<option value="S" selected="selected" >Solicitação de Serviço</option>
			</select>
			<?=obrigatorio(); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo da demanda para Sistemas de Informação:</td>
		<td>
			<select name="dmdclassificacaosistema" id="dmdclassificacaosistema" >
				<option value=""> -- Selecione -- </option>
				<?php if($_SESSION['sisid'] == 15): ?>					
					<option value="4">Manutenção corretiva</option>
					<option value="5">Manutenção evolutiva</option>
				<?php else:  ?>
					<option value="1">Inicial</option>
					<option value="2">Consultiva</option>
					<option value="3" selected="selected" >Investigativa</option>
					<option value="4">Manutenção corretiva</option>
					<option value="5">Manutenção evolutiva</option>
				<?php endif; ?>				
			</select>
			<?=obrigatorio(); ?>			
		</td>
	</tr>
	<?php if($_SESSION['sisid'] == 15): ?>
		<tr>
			<td align='right' class="SubTituloDireita">Prioridade:</td>
			<td>
				<?php
				$sql = "SELECT 
							priid AS codigo, 
							pridsc AS descricao
		  				FROM 
		  					demandas.prioridade
						WHERE
		  					pristatus = 'A'
						ORDER BY
							priid";
					
				$db->monta_combo( "priid", $sql, "S", "", "", "", "", "", "N", "priid" );
				?>
			</td>
		</tr>
	<?php endif;  ?>
	<tr bgcolor="#C0C0C0">
		<td width="15%">&nbsp;</td>
		<td>
		<input type="submit" class="botao" name="btalterar" value="Enviar">
		</td>
	</tr>
</table>

</form>