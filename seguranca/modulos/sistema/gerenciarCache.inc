<?php 
function salvarRegraCache($dados)
{
	global $db;
	extract($dados);
	$sql = "select cacid from seguranca.cache where mnuid = $mnuid";
	$cacid = $db->pegaUm($sql);
	
	$cacexpira = str_replace(".","",$cacexpira);
	$cacrefazercache = !$cacrefazercache ? "false" : $cacrefazercache;
	$cacnaofazercache = !$cacnaofazercache ? "false" : $cacnaofazercache;
	$cacstatus = !$cacstatus ? "A" : $cacstatus;
	
	if($cacid){
		$sql = "update 
					seguranca.cache
				set
					cactipoexpira = '$cactipoexpira',
					cacexpira = $cacexpira,
					cacrefazercache = $cacrefazercache,
					cacnaofazercache = $cacnaofazercache,
					cacstatus = '$cacstatus',
					cacdata = now()
				where
					cacid = $cacid;";
	}else{
		$sql = "insert into 
					seguranca.cache 
				(mnuid,cactipoexpira,cacexpira,cacrefazercache,cacnaofazercache,cacstatus) 
					values 
				($mnuid,'$cactipoexpira',$cacexpira,$cacrefazercache,$cacnaofazercache,'$cacstatus')";
	}
	
	$db->executar($sql);
	$db->commit();
}

function salvarCacheParametro($dados)
{
	global $db;
	extract($dados);
	
	$capexpira = str_replace(".","",$capexpira);
	$caprefazercache = !$caprefazercache ? "false" : $caprefazercache;
	$capnaofazercache = !$capnaofazercache ? "false" : $capnaofazercache;
	$capstatus = !$capstatus ? "A" : $capstatus;
	$capdescricao = !$capdescricao ? "null" : "'$capdescricao'";
	
	$sql = "update 
					seguranca.cacheparametro
				set
					capdescricao = $capdescricao,
					captipoexpira = '$captipoexpira',
					capexpira = $capexpira,
					caprefazercache = $caprefazercache,
					capnaofazercache = $capnaofazercache,
					capstatus = '$capstatus'
				where
					capid = $capid;";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>window.opener.document.getElementById('formulario_cache').submit();window.close();</script>";
	
}

function alterarCacheParametro($dados)
{
	global $db;
	extract($dados);
	$sql = "select * from seguranca.cacheparametro where capid = $capid";
	$arrCap = $db->pegaLinha($sql);
	if($arrCap){
		extract($arrCap);
	}
	?>
	<html>
		<head>
			<title><?php echo NOME_SISTEMA; ?></title>
			<script type="text/javascript" src="../includes/funcoes.js"></script>
		    <script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		    <style>
				.SubtituloTabela{background-color:#cccccc}
				.negrito{font-weight:bold}
				.bold{font-weight:bold}
				.normal{font-weight:normal}
				.center{text-align: center;}
				.direita{text-align: right;}
				.esquerda{text-align: left;}
				.msg_erro{color:#990000}
				.link{cursor: pointer}
				.mini{width:12px;height:12px}
				.sucess_msg{color: blue;}
				.img_middle{vertical-align:middle}
				.hidden{display:none}
				.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
				.fechar{position:relative;right:-5px;top:-26px;}
				.img{background-color:#FFFFFF}
				.red{color:#990000}
			</style>
			<script>
				function salvarCacheParametro()
				{
					if( !jQuery("[name='capexpira']").val() || !jQuery("[name='captipoexpira']").val() ){
						alert('Informe o tempo de expiração do cache!')
					}else{
						jQuery("#formulario_cache").submit();
					}	
				}
			</script>
		</head>
		<body>
			<?php monta_titulo( "Gerenciar Cache", '&nbsp' ); ?>
			<form name="formulario_cache" id="formulario_cache"  method="post" action="" >
			<input type="hidden" name="requisicao" value="salvarCacheParametro" />
			<input type="hidden" name="capid" value="<?php echo $capid ?>" />
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td class="SubtituloDireita" width="25%">Descrição</td>
						<td><?php echo campo_textarea('capdescricao','N','S','',80,5,255,'','','','','',$capdescricao) ?></td>
					</tr>
					<tr>
						<td class="SubtituloDireita" width="25%">Requisição</td>
						<td><?php 
								  echo campo_textarea('caprequisicao','N','N','',100,10,1000,'','','','','',$caprequisicao) ?></td>
					</tr>
					<tr>
						<td class="SubtituloDireita" width="25%">Expiram em</td>
						<td>
							<?php echo campo_texto("capexpira","S","S","",10,10,"[.###]","","right","","","","",$capexpira) ?> 
							<?php 
								$arrDados = array( 
													0=> array("codigo" => "D","descricao"=>"dia(s)"),
													1=> array("codigo" => "H","descricao"=>"hora(s)"),
													2=> array("codigo" => "M","descricao"=>"minuto(s)"),
													3=> array("codigo" => "S","descricao"=>"segundo(s)") 
												  );
								$db->monta_combo('captipoexpira',$arrDados,'S','Selecione','','',"","","","","",$captipoexpira);
							?>
						</td>
					</tr>
					<tr>
						<td class="SubtituloDireita" >Excluir cache?</td>
						<td>
							<?php
								$caprefazercache = $caprefazercache == "t" ? "true" : "false"; 
								$arrDados = array( 
													0=> array("codigo" => "true","descricao"=>"Sim"),
													1=> array("codigo" => "false","descricao"=>"Não")
												  );
								$db->monta_combo('caprefazercache',$arrDados,'S','Selecione','','',"","","","","",$caprefazercache);
							?>
						</td>
					</tr>
					<tr>
						<td class="SubtituloDireita" >Fazer cache?</td>
						<td>
							<?php 
								$capnaofazercache = $capnaofazercache == "t" ? "true" : "false";
								$arrDados = array( 
													0=> array("codigo" => "true","descricao"=>"Sim"),
													1=> array("codigo" => "false","descricao"=>"Não")
												  );
								$db->monta_combo('capnaofazercache',$arrDados,'S','Selecione','','',"","","","","",$capnaofazercache);
							?>
						</td>
					</tr>
					<tr>
						<td class="SubtituloDireita" >Status</td>
						<td>
							<?php 
								$arrDados = array( 
													0=> array("codigo" => "A","descricao"=>"Ativo"),
													1=> array("codigo" => "I","descricao"=>"Inativo")
												  );
								$db->monta_combo('capstatus',$arrDados,'S','Selecione','','',"","","","","",$capstatus);
							?>
						</td>
					</tr>
					<tr>
						<td class="SubtituloTabela center" colspan="2" >
							<input type="button" name="btn_salvar" value="Salvar" onclick="salvarCacheParametro()" />
						</td>
					</tr>
				</table>
			</form>
		</body>
	</html>
	<?php
	exit;
}

function consultarCacheParametro($dados)
{
	global $db;
	extract($dados);
	$sql = "select 
				caprequisicao,sisdiretorio,mnu.mnuid
			from
				seguranca.cacheparametro cap
			inner join
				seguranca.cache cac ON cac.cacid = cap.cacid
			inner join
				seguranca.menu mnu ON cac.mnuid = mnu.mnuid
			inner join
				seguranca.sistema sis ON mnu.sisid = sis.sisid
			where
				cap.capid = $capid";
	$arrDados = $db->pegaLinha($sql);
	$arrParametros = json_decode($arrDados['caprequisicao']);
	
	if(is_object($arrParametros->SESSION)){ // Loop no $_SESSION tratado para gerar a hash do cache
		foreach($arrParametros->SESSION as $keyCache => $arr){
			$string .= $keyCache.$arr;
		}
	}
	if(is_object($arrParametros->POST)){ // Loop no $_POST para gerar a hash do cache
		foreach($arrParametros->POST as $keyCache => $arr){
			$string .= $keyCache.$arr;
		}
	}
	if(is_object($arrParametros->GET)){ // Loop no $_GET para gerar a hash do cache
		foreach($arrParametros->GET as $keyCache => $arr){
			$string .= $keyCache.$arr;
		}
	}

	$cacheFile = md5($string); //Nome do arquivo que armazenará o conteúdo.
	if(is_file(APPRAIZ."arquivos/".$arrDados['sisdiretorio']."/cache/".$arrDados['mnuid']."/$cacheFile.html")){
		include(APPRAIZ."arquivos/".$arrDados['sisdiretorio']."/cache/".$arrDados['mnuid']."/$cacheFile.html");
	}else{
		echo "Não foi possível recuperar o cache em '".APPRAIZ."arquivos/".$arrDados['sisdiretorio']."/cache/".$arrDados['mnuid']."/$cacheFile.html'".".";
	}
	exit;
}

if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']($_REQUEST);
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = !$titulo ? str_replace("...","",$_SESSION['mnudsc']) : $titulo;
monta_titulo( $titulo, '&nbsp' );

if($_POST['mnuid']){
	$sql = "select * from seguranca.cache where mnuid = {$_POST['mnuid']}";
	$arrDados = $db->pegaLinha($sql);
	if($arrDados){
		extract($arrDados);
	}
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
	function selecionaSistema()
	{
		jQuery("#formulario_cache").submit();
	}

	function salvarRegraCache()
	{
		if( !jQuery("[name='cacexpira']").val() || !jQuery("[name='cactipoexpira']").val() ){
			alert('Informe o tempo de expiração do cache!')
		}else{
			jQuery("[name='requisicao']").val("salvarRegraCache");
			jQuery("#formulario_cache").submit();
		}	
	}
	
	function consultarCacheParametro(capid)
	{
		window.open('seguranca.php?modulo=sistema/gerenciarCache&acao=A&requisicao=consultarCacheParametro&capid=' + capid,'scrollbars=no,height=300,width=500,status=no,toolbar=no,menubar=no,location=no');
	}
	
	function alterarCacheParametro(capid)
	{
		janela('seguranca.php?modulo=sistema/gerenciarCache&acao=A&requisicao=alterarCacheParametro&capid=' + capid,800,500,'cache');
	}
	
</script>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.mini{width:12px;height:12px}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle}
	.hidden{display:none}
	.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
	.fechar{position:relative;right:-5px;top:-26px;}
	.img{background-color:#FFFFFF}
	.red{color:#990000}
</style>
<form name="formulario_cache" id="formulario_cache"  method="post" action="" >
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="cacid" value="<?php echo $cacid ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%">Sistema</td>
			<td><?php 
					$sisid = $_POST['sisid'];
					$sql = "select sisid as codigo, sisabrev as descricao from seguranca.sistema order by sisabrev"; 
					$db->monta_combo('sisid',$sql,'S','Sistema','selecionaSistema','');
				?>
			</td>
		</tr>
		<?php if($sisid): ?>
			<tr>
				<td class="SubtituloDireita" width="25%">Menu</td>
				<td><?php 
						$mnuid = $_POST['mnuid'];
						$sql = "select mnuid as codigo, mnucod || ' - ' || mnudsc as descricao from seguranca.menu where sisid = $sisid and mnutipo in (2,3) order by mnucod"; 
						$db->monta_combo('mnuid',$sql,'S','Menu','selecionaSistema','');
					?>
				</td>
			</tr>
			<?php if($mnuid): ?>
				<tr>
					<td class="SubtituloDireita" width="25%">Expiram em</td>
					<td>
						<?php echo campo_texto("cacexpira","S","S","",10,10,"[.###]","","right") ?> 
						<?php 
							$arrDados = array( 
												0=> array("codigo" => "D","descricao"=>"dia(s)"),
												1=> array("codigo" => "H","descricao"=>"hora(s)"),
												2=> array("codigo" => "M","descricao"=>"minuto(s)"),
												3=> array("codigo" => "S","descricao"=>"segundo(s)") 
											  );
							$db->monta_combo('cactipoexpira',$arrDados,'S','Selecione','','');
						?>
					</td>
				</tr>
				<tr>
					<td class="SubtituloDireita" >Excluir cache?</td>
					<td>
						<?php
							$cacrefazercache = $cacrefazercache == "t" ? "true" : "false"; 
							$arrDados = array( 
												0=> array("codigo" => "true","descricao"=>"Sim"),
												1=> array("codigo" => "false","descricao"=>"Não")
											  );
							$db->monta_combo('cacrefazercache',$arrDados,'S','Selecione','','');
						?>
					</td>
				</tr>
				<tr>
					<td class="SubtituloDireita" >Fazer cache?</td>
					<td>
						<?php 
							$cacnaofazercache = $cacnaofazercache == "t" ? "true" : "false";
							$arrDados = array( 
												0=> array("codigo" => "true","descricao"=>"Sim"),
												1=> array("codigo" => "false","descricao"=>"Não")
											  );
							$db->monta_combo('cacnaofazercache',$arrDados,'S','Selecione','','');
						?>
					</td>
				</tr>
				<tr>
					<td class="SubtituloDireita" >Status</td>
					<td>
						<?php 
							$arrDados = array( 
												0=> array("codigo" => "A","descricao"=>"Ativo"),
												1=> array("codigo" => "I","descricao"=>"Inativo")
											  );
							$db->monta_combo('cacstatus',$arrDados,'S','Selecione','','');
						?>
					</td>
				</tr>
				<tr>
					<td class="SubtituloTabela center" colspan="2" >
						<input type="button" name="btn_salvar" value="Salvar" onclick="salvarRegraCache()" />
					</td>
				</tr>
			<?php endif; ?>
		<?php endif; ?>
	</table>
</form>
<?php if($cacid): ?>
	<br />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloTabela bold center" >Caches Gerados</td>
		</tr>
	</table>
	<?php $sql = "	select 
						'<span style=\"white-space:nowrap;\" ><img class=\"link\" src=\"../imagens/alterar.gif\" onclick=\"alterarCacheParametro(' || capid || ')\" > <img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"consultarCacheParametro(' || capid || ')\" ></span>' as acao,
						CASE WHEN capdescricao is not null
								THEN capdescricao
								ELSE caprequisicao
						END as dsc,
						'<span style=\"white-space:nowrap;\" >' || to_char(capdata,'DD/MM/YYYY HH:MI:SS') || '</span>' as capdata,
						'<span style=\"white-space:nowrap;\" >' || capexpira || ' ' ||
						CASE 
							WHEN captipoexpira = 'D' THEN 'dia(s)'
							WHEN captipoexpira = 'H' THEN 'hora(s)'
							WHEN captipoexpira = 'M' THEN 'minuto(s)'
							WHEN captipoexpira = 'S' THEN 'segundo(s)'
						END || '</span>' as expira,
						CASE 
							WHEN caprefazercache is true THEN 'Sim'
							ELSE 'Não'
						END as refazer,
						CASE 
							WHEN capnaofazercache is true THEN 'Sim'
							ELSE 'Não'
						END as nuncafazer
					from 
						seguranca.cacheparametro 
					where 
						cacid = $cacid
					and
						capstatus = 'A'";
	$cabecalho = array("Ação","Requisicao","Data","Expira em","Excluir cache?","Fazer cache?"); 
	$db->monta_lista($sql,$cabecalho,100,10,"N","center","N");
	?>
<?php endif; ?>