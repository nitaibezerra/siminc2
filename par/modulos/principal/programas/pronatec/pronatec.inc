<?php
$campo = $_SESSION['par']['muncod'] ? 'muncodpar' 				: 'estufpar';
$valor = $_SESSION['par']['muncod'] ? $_SESSION['par']['muncod'] : $_SESSION['par']['estuf'];
 
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$obPreObraControle 	  	= new PreObraControle();
$obEntidadeParControle 	= new EntidadeParControle();

$muncod = $_SESSION['par']['muncod']; 

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

$boData = true;

$escrita = true;
//$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/cabecalho.inc';

unset($_SESSION['par']['preid']);

$stUF  = $obPreObraControle->recuperarUF($muncod);
$busca = Array( 'campo' => $campo , 'valor' => $valor, 'tipo' => $_REQUEST['tipo'], 'tooid' => 3 );

//$arTermosPronatec  = $obPreObraControle->recuperarTermosPorMuncod($busca, SIS_OBRAS);

if($_GET['excluir']){
	
	if($obPreObraControle->excluirPreObra($_GET['excluir']) && $escrita ){
		echo "<script>
				alert('Operação realizada com sucesso.');
				document.location.href = 'par.php?modulo=principal/programas/pronatec/pronatec&acao=A';
			  </script>";
	}
	exit();
}

$descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['muncod'], null);

?>
<br />
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
<script type="text/javascript" src="../includes/funcoes.js" ></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>		
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript">
	jQuery.noConflict();
</script>
<script type="text/javascript"><!--
jQuery(document).ready(function(){	

	jQuery('.voltar').live('click',function(){

		document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';	
		
	});

	jQuery('.alterar').live('click',function(){

		return window.open('par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&preid='+this.id+'&ano=2011', 
						   'Pronatec', 
						   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();	
		
	});

	jQuery('.excluir').live('click',function(){

		if( confirm("Será excluído todos os dados desta obra, deseja continuar?") ){
			document.location.href = 'par.php?modulo=principal/programas/pronatec/pronatec&acao=A&excluir='+this.id;
		}
		
	});

	jQuery('.adicionar').live('click',function(){

//		var totalObras 		= <?php echo $nrQtdObras ?>;
//		var totalPermitido 	= <?php echo $nrQtdMaxima ?>;
		var escrita = <?=$escrita ?>;
		var boData = <?=($boData) ? "true" : "false" ?>;		
//		if(totalObras < totalPermitido){
			if( escrita &&  boData){
				return window.open('par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&ano=2011&prog=proinf', 
						   'Pronatec',
						   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
			}else{
				alert('Encerrado prazo para inscrições em 29/10/2010.');
			}
//		}else{
//			
//			alert('Número máximo de obras permitidos.');
//		}		
		
	});

	jQuery('#semCobertura').live('click',function(){
		
			var param = new Array();
			param.push({name : 'requisicao', value : 'carga'},
					   {name : 'tipo', value : 'semCobertura'}
					  );
			  
			jQuery.ajax({
				type	 : "POST",
				url		 : "par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A",
				data	 : param,
				async    : false,
				//dataType : 'json',
				success	 : function(data){
							if(data == 1){
								alert("carga concluida");
							}					
						}
				 });
		
	});

});

function focamensagem(){
	document.getElementById('msg').focus();
}

-->
</script>
<?php monta_titulo( 'Pronatec', $descricao  );	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<thead>			
		<tr bgcolor="#e9e9e9" align="center">
			<td width="55px" align="center"><b>Ação</b></td>
			<td align="left"><b>Nome da obra</b></td>
			<td align="left"><b>Tipo de obra</b></td>
			<td align="left"><b>Situação</b></td>
		</tr>
	</thead>
	<tbody id="itensComposicaoSemQuadraCobertura">
	<?php 
		$arPreObrasPronatec	  = $obPreObraControle->recuperarPreObraPorMuncod($busca, SIS_OBRAS);
		$arPreObrasPronatec   = $arPreObrasPronatec ? $arPreObrasPronatec : array();
		
		if(count($arPreObrasPronatec)){ 
			foreach($arPreObrasPronatec as $preobra){ 
			
				$docid = prePegarDocid($preobra['preid']);
				$esdid = prePegarEstadoAtual($docid); 				
				$cor = ($x % 2) ? "#F7F7F7" : "white";
				
		?>					
		<tr bgcolor="<?php echo $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor ?>';" id="<?php echo $preobra['preid'];?>">
			<td style="padding-left:15px;">
				<img src="../imagens/alterar.gif" class="alterar" style="cursor:pointer" id="<?php echo $preobra['preid'] ?>" />
				<img src="../imagens/excluir.gif" class="excluir" style="cursor:pointer" id="<?php echo $preobra['preid'] ?>" />
			</td>				
			<td>						
				<a class="alterar" id="<?php echo $preobra['preid'] ?>"><?php echo $preobra['predescricao'] ?></a>
				<?php if($obPreObraControle->verificaFotosObras($preobra['preid'])){ ?>
					<a href="javascript:void(0)">
						<img class="anexos" id="foto_<?php echo $preobra['preid'] ?>" align="absmiddle" border="0" src="../imagens/cam_foto.gif" title="Fotos cadastradas" />
					</a>							
				<?php } 
					if($obPreObraControle->verificaAnexoObras($preobra['preid'])){ ?>
					<a href="javascript:void(0)">
						<img class="anexos" id="documento_<?php echo $preobra['preid'] ?>" align="absmiddle" border="0" src="../imagens/clipe.gif" title="Documentos anexos" />
					</a>							
				<?php } ?>													
			</td>
			<td>
				<?php echo $preobra['ptodescricao']; ?>							
			</td>
			<td>
			</td>
		</tr>
		<?php $x++;
			}
		}?>	
		<tr>
			<td colspan="4">
				<div>
					<span class="<?= $escrita ? 'adicionar' : ''; ?>" style="cursor:pointer; " >
						<img src="../imagens/gif_inclui<?= $escrita ? '' : '_d'; ?>.gif" border="0" align="absmiddle" /> Inserir imóvel
					</span>
				</div>
			</td>
		</tr>
	</tbody>
</table>
<div id="divDebug"></div>