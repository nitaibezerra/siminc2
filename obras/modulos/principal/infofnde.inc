<?php

if(!$_SESSION['obras']['obrid']) {
	die("<script>
			alert('Obra não identificada');
			window.location='obras.php?modulo=inicio&acao=A';
		 </script>");
}

if($_REQUEST['requisicao'] == 'salvar'){
	
	$infmotivo 			= !empty($_REQUEST['infmotivo']) ? "'".substr($_REQUEST['infmotivo'],0,1000)."'" : 'null';  
	$infprovidencia 	= !empty($_REQUEST['infprovidencia']) ? "'".substr($_REQUEST['infprovidencia'],0,1000)."'" : 'null'; 
	$infretorno 		= !empty($_REQUEST['infretorno']) ? "'".substr($_REQUEST['infretorno'],0,1000)."'" : 'null'; 
	$infacompanhamento  = !empty($_REQUEST['infacompanhamento']) ? "'".substr($_REQUEST['infacompanhamento'],0,1000)."'" : 'null';
	$obrid 				= $_SESSION['obras']['obrid'];
	
	if(empty($_REQUEST['infid'])){
		
		$sql = "insert into obras.infofnde
					(infmotivo, infprovidencia, infretorno, infacompanhamento, obrid, usucpf, infstatus, infdtinclusao)
				values
					({$infmotivo}, {$infprovidencia}, {$infretorno}, {$infacompanhamento}, {$obrid}, '{$_SESSION['usucpf']}', 'A', '".date('Y-m-d')."')";	
	}else{
		
		$sql = "update
					obras.infofnde
				set
					infmotivo = {$infmotivo}, 
					infprovidencia = {$infprovidencia}, 
					infretorno = {$infretorno}, 
					infacompanhamento = {$infacompanhamento}
				where 
					infid = {$_REQUEST['infid']}";
	}
	
	$db->executar($sql);
	if($db->commit()){
		$db->sucesso('principal/infofnde');
	}
	exit;
}

if($_REQUEST['requisicao'] == 'salvarAnexo'){
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$campos	= array("infid" => $_REQUEST['infid']);
	
	$file = new FilesSimec("infofndearquivo", $campos, "obras");
	
	if(is_file($_FILES["arquivo"]["tmp_name"])){		
		$_REQUEST['arqdescricao'] = substr($_REQUEST['arqdescricao'],0,255);
		$arquivoSalvo = $file->setUpload($_REQUEST['arqdescricao']);
		$db->sucesso('principal/infofnde', '&requisicao=popupAnexos&infid='.$_REQUEST['infid']);
	}
	exit;
}

if($_REQUEST['requisicao'] == 'excluir'){
	if(!empty($_REQUEST['infid'])){
		$sql  = " update obras.infofnde set infstatus = 'I' where infid = {$_REQUEST['infid']}; ";
		
		$db->executar($sql);
		if($db->commit()){
			$db->sucesso('principal/infofnde');
		}
	}
	exit;
}

if($_REQUEST['requisicao'] == 'excluirAnexo'){
	if(!empty($_REQUEST['ifaid'])){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$file = new FilesSimec();
	
		$sql  = " delete from obras.infofndearquivo where ifaid = {$_REQUEST['ifaid']}; ";
		$db->executar($sql);
		if($db->commit()){
			$db->sucesso('principal/infofnde', '&requisicao=popupAnexos&infid='.$_REQUEST['infid']);
		}
	}
	exit;
}

if($_REQUEST['requisicao'] == 'carregar'){
	
	if(!empty($_REQUEST['infid'])){
		
		$sql = "select * from obras.infofnde where infid = ".$_REQUEST['infid'];
		$rs = $db->pegaLinha($sql);
		
		echo simec_json_encode($rs);		
	}
	exit;
}

if($_REQUEST['requisicao'] == 'baixarAnexo'){
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$file = new FilesSimec();	
	$file->getDownloadArquivo($_REQUEST['arqid']);
	exit;
}

if($_REQUEST['requisicao'] == 'popupAnexos'){
	
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	      <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	      <script type="text/javascript" src="../includes/funcoes.js"></script>
	      <script type="text/javascript" src="/includes/estouvivo.js"></script>
	      <script type="text/javascript" src="../includes/prototype.js"></script>';
		
	$titulo_modulo 	  = "Anexos";
	$subtitulo_modulo = "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios";
	monta_titulo( $titulo_modulo, $subtitulo_modulo);
	echo "<br/>";
	
	echo '
			<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
			<script>			
				jQuery.noConflict();	

				function salvarAnexo()
				{
					if(jQuery(\'[name=arquivo]\').val() == \'\'){
						alert(\'O campo arquivo é obrigatório!\');
						jQuery(\'[name=arquivo]\').focus();
						return false;
					}					
					jQuery(\'#form_anexo\').submit();
				}
				
				function baixarAnexo(id)
				{
					document.location.href = \'?modulo=principal/infofnde&acao=A&requisicao=baixarAnexo&arqid=\'+id;
				}
				
				function excluirAnexo(id)
				{
					if(confirm(\'Deseja excluir o anexo?\')){
						document.location.href = \'?modulo=principal/infofnde&acao=A&requisicao=excluirAnexo&ifaid=\'+id+\'&infid=\'+jQuery(\'[name=infid]\').val();
					}
				}
			</script>
		 ';
	
	if(!possuiPerfil(PERFIL_ADMINISTRADOR)){
	
		$sql = "select usucpf from obras.infofnde where usucpf = '{$_SESSION['usucpf']}' and infid = {$_REQUEST['infid']}";
		$rs = $db->pegaUm($sql);
		
		if(!$rs){
			$ativo = 'N';
		}
		
	}else{
		
		$rs = true;
	}
	
	echo '
			<form method="post" name="form_anexo" id="form_anexo" action="" enctype="multipart/form-data">
				<input type="hidden" name="requisicao" value="salvarAnexo" />
				<input type="hidden" name="ifaid" value="" />
				<input type="hidden" name="infid" value="'.$_REQUEST['infid'].'" />
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
					<tr>
						<td class="subtituloDireita" width="200">Arquivo</td>
						<td>
							<input type="file" name="arquivo" '.($rs ? '' : 'disabled="disabled"').'/>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita" width="200">Descrição</td>
						<td>
							'.campo_textarea('arqdescricao', 'N', $ativo, '', 60, 3, 255).'
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">&nbsp;</td>
						<td>
							<input type="button" value="Salvar" onclick="salvarAnexo();" '.($rs ? '' : 'disabled="disabled"').'/>
							<input type="button" value="Fechar" onclick="javascript:window.close();" />
						</td>
					</tr>
				</table>
			</form>
		 ';
	
	$btAcao1  = '<img src="../imagens/lupa_grafico.gif" onclick="baixarAnexo(\' || a.arqid || \')" style="cursor:pointer;" title="Visualizar" alt="Visualizar"/>';
	$btAcao1 .= '&nbsp;<img src="../imagens/excluir.gif" onclick="excluirAnexo(\' || a.ifaid || \')" style="cursor:pointer;" title="Excluir" alt="Excluir"/>';	
	
	$btAcao2  = '<img src="../imagens/lupa_grafico.gif" onclick="baixarAnexo(\' || a.arqid || \')" style="cursor:pointer;" title="Visualizar" alt="Visualizar"/>';
	$btAcao2 .= '&nbsp;<img src="../imagens/excluir_01.gif"/>';
	
if(possuiPerfil(PERFIL_ADMINISTRADOR)){
	$stAcao = "'".$btAcao1."' as acao,";
}else{
	$stAcao = "case when i.usucpf = '{$_SESSION['usucpf']}' then '{$btAcao1}'
			   else '{$btAcao2}' end as acao,";
}
	
	$sql = "select
				{$stAcao}
				'<a href=\"javascript:void(0)\" onclick=\"baixarAnexo(' || a.arqid || ')\" title=\"' || p.arqdescricao || '\" alt=\"' || p.arqdescricao || '\">' || arqnome || '.' || arqextensao || '</a>' as arquivo,				
				to_char(arqdata::date,'DD/MM/YYYY') 
			from 
				obras.infofndearquivo a 
			join 
				obras.infofnde i on i.infid = a.infid
			join 
				public.arquivo p on p.arqid = a.arqid 
			where
				i.infid = {$_REQUEST['infid']}
			order by 
				ifaid desc";
	
	$cabecalho = array('Ação','Arquivo','Data');
	$db->monta_lista($sql, $cabecalho, 10, 10, 'N', '', 'N', 'listaInfofnde', '', '');
	exit;
}

require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

$ativo = 'S';

$abacod_tela = 57024;
montaAbaObras($abacod_tela,$url,$parametros);

// Cria o título da tela
$titulo_modulo 	  = "Informações FNDE";
$subtitulo_modulo = "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios";
monta_titulo( $titulo_modulo, $subtitulo_modulo);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

jQuery.noConflict();

function salvarDados()
{
	if(jQuery('#infmotivo').val() == ''){
		alert('O campo motivo é obrigatório!');
		jQuery('#infmotivo').focus();
		return false;
	}
	
	jQuery('#formulario').submit();
}

function carregarDados(id)
{
	jQuery.ajax({
		url: '?modulo=principal/infofnde&acao=A',
		type: 'post',
		dataType: 'json',
		data: 'requisicao=carregar&infid='+id,
		success: function(e){
			jQuery('[name=infid]').val(e.infid);
			jQuery('[name=infmotivo]').val(e.infmotivo);
			jQuery('[name=infprovidencia]').val(e.infprovidencia);
			jQuery('[name=infretorno]').val(e.infretorno);
			jQuery('[name=infacompanhamento]').val(e.infacompanhamento);
		}
	});	
}

function excluirDados(id)
{
	if(confirm('Deseja deletar o registro?')){
		document.location.href = '?modulo=principal/infofnde&acao=A&requisicao=excluir&infid='+id;
	}
}

function popupAnexos(id)
{
	var popUp = window.open('?modulo=principal/infofnde&acao=A&requisicao=popupAnexos&infid='+id, 'popupAnexos', 'height=400,width=600,scrollbars=yes,top=50,left=200');
	popUp.focus();
}
</script>
<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="requisicao" value="salvar" />
	<input type="hidden" name="infid" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
		<tr>
			<td class="subtituloDireita" width="200">Motivo</td>
			<td><?php echo campo_textarea('infmotivo', 'S', $ativo, '', 80, 3, 1000) ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Providência</td>
			<td><?php echo campo_textarea('infprovidencia', 'N', $ativo, '', 80, 3, 1000) ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Retorno</td>
			<td><?php echo campo_textarea('infretorno', 'N', $ativo, '', 80, 3, 1000) ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Acompanhamento</td>
			<td><?php echo campo_textarea('infacompanhamento', 'N', $ativo, '', 80, 3, 1000) ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td>
				<input type="button" value="Salvar" onclick="salvarDados()" <?php echo $ativo != 'S' ? 'disabled="disabled"' : '' ?>/>			
			</td>
		</tr>
	</table>
</form>
<?php

$btAcao1  = '<img src="../imagens/alterar.gif" onclick="carregarDados(\' || infid || \')" style="cursor:pointer;" title="Alterar" alt="Alterar"/>';
$btAcao1 .= '&nbsp;<img src="../imagens/clipe1.gif" onclick="popupAnexos(\' || infid || \')" style="cursor:pointer;" title="Abrir anexos" alt="Abrir anexos"/>';
$btAcao1 .= '&nbsp;<img src="../imagens/excluir.gif" onclick="excluirDados(\' || infid || \')" style="cursor:pointer;" title="Excluir" alt="Excluir"/>';

$btAcao2  = '<img src="../imagens/alterar_01.gif" />';
$btAcao2 .= '&nbsp;<img src="../imagens/clipe1.gif" onclick="popupAnexos(\' || infid || \')" style="cursor:pointer;" title="Abrir anexos" alt="Abrir anexos"/>';
$btAcao2 .= '&nbsp;<img src="../imagens/excluir_01.gif"/>';

if(possuiPerfil(PERFIL_ADMINISTRADOR)){
	$stAcao = "'".$btAcao1."' as acao,";
}else{
	$stAcao = "case when i.usucpf = '{$_SESSION['usucpf']}' then '{$btAcao1}'
			   else '{$btAcao2}' end as acao,";
}

$sql = "select			
			{$stAcao}			
			i.infmotivo,
			i.infprovidencia,
			i.infretorno,
			i.infacompanhamento,
			u.usunome,
			to_char(i.infdtinclusao::date,'DD/MM/YYYY') 
		from 
			obras.infofnde i
		left join 
			seguranca.usuario u on u.usucpf = i.usucpf
		where
			i.obrid = ".$_SESSION['obras']['obrid']. " and i.infstatus = 'A'
		order by 
			i.infid desc";
$cabecalho = array('Ação','Motivo', 'Providência', 'Retorno', 'Acompanhamento', 'Responsável', 'Data');
$db->monta_lista($sql, $cabecalho, 10, 10, 'N', '', 'N', 'listaInfofnde', '', '');
?>