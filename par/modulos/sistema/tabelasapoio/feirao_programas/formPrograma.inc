<?php 

if($_REQUEST['popup']){
	include APPRAIZ . 'par/modulos/sistema/tabelasapoio/feirao_programas/'.$_REQUEST['popup'].'.inc';
	die; 
}

if($_REQUEST['requisicao'] == 'verificaEsfera'){
	
	$_REQUEST['tprcod'] = $_REQUEST['tprcod'] ? ' tprcod = '.$_REQUEST['tprcod'] : ' tprcod is null ';
	
	$sql = "select 
				tapid 
			from 
				par.pftermoadesaoprograma 
			where 
				prgid = {$_REQUEST['prgid']} 
			and 
				 {$_REQUEST['tprcod']} 
			and 
				tapstatus = 'A'";
	
	$tpaid = $db->pegaUm($sql);
	
	if($tpaid){
		if($_REQUEST['tprcod'] == 1){
			die('Estadual');
		}else if($_REQUEST['tprcod'] == 2){
			die('Municipal');
		}else{
			die('Todas');
		}
	}	
	die;
}

if($_REQUEST['requisicao'] == 'salvarAba'){
	
	$_REQUEST['funid'] = $_REQUEST['funid'] ? $_REQUEST['funid'] : 'null';
	$_REQUEST['pfclimiteregistros'] = $_REQUEST['pfclimiteregistros'] ? $_REQUEST['pfclimiteregistros'] : 'null';
	$_REQUEST['pfcidsfuncaoatual'] = is_array($_REQUEST['pfcidsfuncaoatual']) ? implode(',',$_REQUEST['pfcidsfuncaoatual']) : '';
	$_REQUEST['pfcidsformacao'] = is_array($_REQUEST['pfcidsformacao']) ? implode(',',$_REQUEST['pfcidsformacao']) : '';
	
	if($_REQUEST['pfcid']){
		
		$sql = "update par.pfcurso set
					pfcdescricao 		= '{$_REQUEST['pfcdescricao']}',					
					funid 				= {$_REQUEST['funid']},
					pfctitulo 			= '{$_REQUEST['pfctitulo']}',
					pfclimiteregistros 	= {$_REQUEST['pfclimiteregistros']},
					pfcorientacoes 		= '{$_REQUEST['pfcorientacoes']}',
					pfcscripts 			= '{$_REQUEST['pfcscripts']}',
					pfcidsfuncaoatual 	= '{$_REQUEST['pfcidsfuncaoatual']}',
					pfcidsformacao 		= '{$_REQUEST['pfcidsformacao']}',
					pfcmsgsucesso 		= '{$_REQUEST['pfcmsgsucesso']}'
  				where
  					pfcid = {$_REQUEST['pfcid']}";
		
	}else{
		
		$sql = "insert into par.pfcurso
					(prgid,pfcdescricao,pfcstatus,funid,pfctitulo,pfclimiteregistros,pfcorientacoes,pfcscripts,pfcidsfuncaoatual,pfcidsformacao,pfcmsgsucesso)
				values
					({$_REQUEST['prgid']}, '{$_REQUEST['pfcdescricao']}', 'A', {$_REQUEST['funid']}, '{$_REQUEST['pfctitulo']}',{$_REQUEST['pfclimiteregistros']},
					'{$_REQUEST['pfcorientacoes']}', '{$_REQUEST['pfcscripts']}', '{$_REQUEST['pfcidsfuncaoatual']}', '{$_REQUEST['pfcidsformacao']}', 
					'{$_REQUEST['pfcmsgsucesso']}');
					";
	}
	
	$db->executar($sql);
	
	if($db->commit()){
		$db->sucesso('sistema/tabelasapoio/feirao_programas/formPrograma', '&prgid='.$_REQUEST['prgid']);
	}	
}

if($_REQUEST['requisicao'] == 'salvarTermo'){
	
	$_REQUEST['tapano'] = $_REQUEST['tapano'] ? $_REQUEST['tapano'] : 'null';
	$_REQUEST['tprcod'] = trim($_REQUEST['tprcod']) ? $_REQUEST['tprcod'] : 'null';
	$_REQUEST['tpdid'] = $_REQUEST['tpdid'] ? $_REQUEST['tpdid'] : 'null';
	
	if($_REQUEST['tapid']){
		
		$sql = "update par.pftermoadesaoprograma set 
					tapano 					= {$_REQUEST['tapano']},
					taptermo 				= '{$_REQUEST['taptermo']}',
					tprcod 					= {$_REQUEST['tprcod']},
					tapinstrucao 			= '{$_REQUEST['tapinstrucao']}',
					tapmsg 					= '{$_REQUEST['tapmsg']}',
					tapconteudoemail 		= '{$_REQUEST['tapconteudoemail']}',
					tappacto 				= '{$_REQUEST['tappacto']}',
					tapmsgaceitetermo 		= '{$_REQUEST['tapmsgaceitetermo']}',
					tapmsgaceitepacto 		= '{$_REQUEST['tapmsgaceitepacto']}',
					tapmsgnaoaceitetermo 	= '{$_REQUEST['tapmsgnaoaceitetermo']}',
					tapmsgnaoaceitepacto 	= '{$_REQUEST['tapmsgnaoaceitepacto']}',
					tapredireciona 			= '{$_REQUEST['tapredireciona']}',
					tpdid 					= {$_REQUEST['tpdid']},
					taptituloadesao 		= '{$_REQUEST['taptituloadesao']}'
				where 
					tapid = {$_REQUEST['tapid']}";
		
	}else{
		
		$sql = "insert into par.pftermoadesaoprograma
					(prgid,tapano,tapstatus,taptermo,tprcod,tapinstrucao,tapmsg,tapconteudoemail,tappacto,tapmsgaceitetermo,tapmsgaceitepacto,tapmsgnaoaceitetermo,
					tapmsgnaoaceitepacto,tapredireciona,tpdid,taptituloadesao)
				values
					({$_REQUEST['prgid']}, {$_REQUEST['tapano']}, 'A', '{$_REQUEST['taptermo']}', {$_REQUEST['tprcod']}, '{$_REQUEST['tapinstrucao']}', '{$_REQUEST['tapmsg']}',
					'{$_REQUEST['tapconteudoemail']}', '{$_REQUEST['tappacto']}', '{$_REQUEST['tapmsgaceitetermo']}', '{$_REQUEST['tapmsgaceitepacto']}', '{$_REQUEST['tapmsgnaoaceitetermo']}',
					'{$_REQUEST['tapmsgnaoaceitepacto']}', '{$_REQUEST['tapredireciona']}', {$_REQUEST['tpdid']}, '{$_REQUEST['taptituloadesao']}');";
		
	}
	
	$db->executar($sql);
	
	if($db->commit()){
		$db->sucesso('sistema/tabelasapoio/feirao_programas/formPrograma', '&prgid='.$_REQUEST['prgid']);
	}	
}

if($_REQUEST['requisicao'] == 'salvaDados'){
	
	$_REQUEST['pfaano'] 		= $_REQUEST['pfaano'] ? $_REQUEST['pfaano'] : 'null';
	$_REQUEST['pfadatainicial'] = $_REQUEST['pfadatainicial'] ? formata_data_sql($_REQUEST['pfadatainicial']) : '';
	$_REQUEST['pfadatafinal'] 	= $_REQUEST['pfadatafinal'] ? formata_data_sql($_REQUEST['pfadatafinal']) : '';
	
	if($_REQUEST['prgid']){
		
		$sql = "update par.programa set prgdsc = '{$_REQUEST['prgdsc']}' where prgid = {$_REQUEST['prgid']};";
		$sql .= "update par.pfadesao set 
					pfadatainicial = '{$_REQUEST['pfadatainicial']}',
					pfadatafinal = '{$_REQUEST['pfadatafinal']}',
					pfaano = {$_REQUEST['pfaano']},
					pfaesfera = '{$_REQUEST['pfaesfera']}'					
				where
					prgid = {$_REQUEST['prgid']};";
		
		$prgid = $_REQUEST['prgid'];
		
	}else{
		
		$sql = "insert into par.programa
					(prgdsc, prgstatus)
				values
					('{$_REQUEST['prgdsc']}', 'A') returning prgid;";
		
		$prgid = $db->pegaUm($sql);
		
		$sql = "insert into par.pfadesao
					(pfadatainicial, pfadatafinal, pfaano, pfaesfera, prgid, pfaicone)
				values 
					('{$_REQUEST['pfadatainicial']}','{$_REQUEST['pfadatafinal']}',{$_REQUEST['pfaano']},'{$_REQUEST['pfaesfera']}', $prgid, ' ');";
		
	}	
	
	$db->executar($sql);
	
	if($db->commit()){
		$db->sucesso('sistema/tabelasapoio/feirao_programas/formPrograma', '&prgid='.$prgid);
	}	
}

if($_REQUEST['requisicao'] == 'salvarImagem'){
	
	$type = $_FILES['arquivo']['type'];
	$image = $_FILES["arquivo"]["name"];
	$uploadedfile = $_FILES['arquivo']['tmp_name'];
	$nomeImagem = 'simbolo_prgid_'.$_REQUEST['prgid'] . '.jpg';
	
	if ($image){

		if($type == 'image/png'){						
			$src = imagecreatefrompng($uploadedfile);
		}else		
		if($type == 'image/gif'){
			$src = imagecreatefromgif($uploadedfile);
		}else 
		if($type == 'image/jpeg'){
			$src = imagecreatefromjpeg($uploadedfile);
		}
		
		list($width,$height) = getimagesize($uploadedfile);
		
//		$newwidth=60;
//		$newheight=($height/$width)*$newwidth;

		$newwidth=142;
		$newheight=105;
		
		$tmp=imagecreatetruecolor($newwidth,$newheight);
		
		imagecopyresampled($tmp,$src,0,0,0,0,$newwidth,$newheight,$width,$height);
		
		$dirname = APPRAIZ."/arquivos/proinfantil/feirao_programas/";
		
		if(!is_dir($dirname)){
			mkdir($dirname, 0777);
		}
		
		$filename = $dirname . $nomeImagem;
		
		if(file_exists($filename)){
			unlink($filename);			
		}
		
		imagejpeg($tmp,$filename,100);
		
		imagedestroy($src);
		imagedestroy($tmp);
		
	}
	
	$sql = "update par.pfadesao set pfaicone = '{$nomeImagem}' where prgid = ".$_REQUEST['prgid'];
	$db->executar($sql);
	
	if($db->commit()){
		$db->sucesso('sistema/tabelasapoio/feirao_programas/formPrograma', '&prgid='.$_REQUEST['prgid']);
	}
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

$titulo1 = "Cadastrar Programa";
if($_REQUEST['prgid']){
	$titulo1 = "Editar Programa";	
}
$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';

monta_titulo($titulo1, $titulo2);

if($_REQUEST['prgid']){
	
	$sql = "select 
				prg.prgid,
				prg.prgdsc,
				pfa.pfadatainicial,
				pfa.pfadatafinal,
				pfa.pfaano,
				pfa.pfaesfera,
				pfa.pfaicone,
				pfa.pfaid
			from 
				par.programa prg
			left join 
				par.pfadesao pfa on pfa.prgid = prg.prgid
			where 
				prg.prgid = {$_REQUEST['prgid']}";
	
	$rs = $db->pegaLinha($sql);
	
	if($rs) extract($rs);
}

if($pfadatainicial){
	$pfadatainicial = formata_data($pfadatainicial);
}
if($pfadatafinal){
	$pfadatafinal = formata_data($pfadatafinal);
}

?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<style>
	.ui-dialog-titlebar{ display: none; }
	.dialog-confirm{ padding:0px; margin:0px; }
	.dialog-content{ padding:0px; margin:0px; }
	/*
	#accordion a{ color: black; }
	#accordion a:hover{ color: blue; }
	.ui-icon{ color:black }
	*/
</style>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<!--<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>-->
<script language="javascript" type="text/javascript" src="../includes/tinymce_jquery/jscripts/tiny_mce/tiny_mce.js"></script>
<script>

	jQuery.noConflict();
	
	jQuery(function(){

		jQuery("[name=prgdsc]").addClass("required");
		jQuery("[name=pfadatainicial]").addClass("required");
		jQuery("[name=pfadatafinal]").addClass("required");
		jQuery("[name=pfaano]").addClass("required");
		jQuery("[name=pfaesfera]").addClass("required");	
		
		jQuery("#formPrograma").validate();
		
		jQuery('#btnSalvar').click(function(){
			jQuery('[name=requisicao]').val('salvaDados');
			jQuery('#formPrograma').submit();
		});
		
	});

	function modalCurso(pfcid)
	{
		
		var prgid = jQuery('[name=prgid]').val(); 

		jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
		jQuery( '#dialog-confirm' ).dialog({
			resizable: true,
			width: 800,
			height:640,
			modal: true,
			open: function (event, ui) {
								
				var url = 'par.php?modulo=sistema/tabelasapoio/feirao_programas/formPrograma&acao=A&popup=formAbas&pfcid='+pfcid+'&prgid='+prgid;
				
				jQuery('#dialog-content').load(url,function(){

					tinyMCE.init({
						
				        // General options				        
				        mode 	 : "exact",
				        width 	 : "100%",	
				        height 	 : "100%",						        
						elements : "pfcorientacoes",						
				        theme 	 : "advanced",
				        plugins  : "pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template",

				        // Theme options				        
				        theme_advanced_buttons1 : "bold,italic,underline,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect,|,forecolor,backcolor,|,link,unlink,image,code,fullscreen",
				        theme_advanced_buttons2 : "",
				        theme_advanced_buttons3 : "",
				        theme_advanced_buttons4 : "",
				        theme_advanced_toolbar_location : "top",
				        theme_advanced_toolbar_align : "left",
				        theme_advanced_statusbar_location : "bottom",
				        theme_advanced_resizing : false,
				        
					});
									
				});
			}
		});
		
	}

	function modalTermo(tapid)
	{
		var prgid = jQuery('[name=prgid]').val(); 

		jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
		jQuery( '#dialog-confirm' ).dialog({
			resizable: true,
			width: 800,
			height:655,
			modal: true,
			open: function (event, ui) {
								
				var url = 'par.php?modulo=sistema/tabelasapoio/feirao_programas/formPrograma&acao=A&popup=formTermo&tapid='+tapid+'&prgid='+prgid;
				
				jQuery('#dialog-content').load(url,function(){

					tinyMCE.init({
						
				        // General options				        
				        mode 	 : "exact",
				        width 	 : "100%",	
				        height 	 : "100%",					        				        
						elements : "tappacto,taptermo",						
				        theme 	 : "advanced",
				        plugins  : "pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template",

				        // Theme options				        
				        theme_advanced_buttons1 : "bold,italic,underline,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect,|,forecolor,backcolor,|,link,unlink,image,code,fullscreen",
				        theme_advanced_buttons2 : "",
				        theme_advanced_buttons3 : "",
				        theme_advanced_buttons4 : "",
				        theme_advanced_toolbar_location : "top",
				        theme_advanced_toolbar_align : "left",
				        theme_advanced_statusbar_location : "bottom",
				        theme_advanced_resizing : false,
				        
					});
					
				});
			}
		});
	}
	
</script>
<form name="formPrograma" id="formPrograma" method="post" action="">
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="prgid" value="<?php echo $prgid ? $prgid : ''; ?>" />
	<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<?php if($pfaicone && false): ?>
			<tr>
				<td class="subtituloDireita">Logo</td>
				<td><img src="../imagens/<?php echo $pfaicone; ?>" /></td>				
			</tr>	
		<?php endif; ?>
		<tr>
			<td class="subtituloDireita">Nome do Programa</td>
			<td>
				<?php echo campo_texto('prgdsc', 'S', 'S', '', 65, 255, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Período</td>
			<td>
				<?php echo campo_data('pfadatainicial', 'S', 'S', '', 'N'); ?>
				&nbsp;até&nbsp;
				<?php echo campo_data('pfadatafinal', 'S', 'S', '', 'N'); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Ano de Referência</td>
			<td>
				<?php echo campo_texto('pfaano', 'S', 'S', '', 10, 4, '####', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Esfera</td>
			<td>
				<?php		
						 
					$arDados = array(
									array('codigo'=>'', 'descricao'=>'Selecione...'),
									array('codigo'=>'M', 'descricao'=>'Municipal'),
									array('codigo'=>'E', 'descricao'=>'Estadual'),
									array('codigo'=>'T', 'descricao'=>'Todos')
								);		
										
					$db->monta_combo('pfaesfera', $arDados, 'S', '', 'Selecione...', '', '', '', 'S');
									
				?>
			</td>
		</tr>		
		<tr>
			<td colspan="2" class="subtituloCentro">Termo Adesão</td>
		</tr>
		<tr>
			<td colspan="2">
				<?php 
				
					if($_REQUEST['prgid']){
						$sql = "select 
									tapid,
									case when tprcod = 1 then 'Estadual'
										 when tprcod = 2 then 'Municipal'
									else 'Todas' end as tprcod,
									tappacto,
									taptermo
								from 
									par.pftermoadesaoprograma 
								where 
									prgid = {$_REQUEST['prgid']}
								and
									tapstatus = 'A'";
						
						$rsTermos = $db->carregar($sql);
					}
					$rsTermos = $rsTermos ? $rsTermos : array();
				
				?>
				<table id="tb_termo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<th>Ações</th>
							<th>Esfera</th>
							<th>Possui pacto</th>							
							<th>Possui termo</th>
						</tr>
					</thead>
					<tbody>
						<?php $x=0; ?>
						<?php foreach($rsTermos as $termo):?>
							<?php $cor = $x % 2 ? '#f0f0f0' : 'white'; ?>
							<tr style="background:<?php echo $cor; ?>">
								<td align="center" width="80">
									<img src="../imagens/alterar.gif" style="cursor:pointer" alt="Editar" title="Editar" onclick="modalTermo(<?php echo $termo['tapid']?>)"/>
								</td>
								<td align="center"><?php echo $termo['tprcod']; ?></td>
								<td align="center" width="200"><?php echo !empty($termo['tappacto']) ? '<img src="../imagens/check_p.gif" />' : '<img src="../imagens/exclui_p.gif" />'; ?></td>
								<td align="center" width="200"><?php echo !empty($termo['taptermo']) ? '<img src="../imagens/check_p.gif" />' : '<img src="../imagens/exclui_p.gif" />'; ?></td>								
							</tr>
							<?php $x++; ?>			
						<?php endforeach; ?>
					</tbody>
					<?php if($prgid): ?>
						<tfoot>
							<tr>
								<td colspan="4" style="border-top: 0px solid black;">
									<a href="javascript:void(0)" onclick="modalTermo('')">
										<img src="../imagens/gif_inclui.gif" align="absmiddle"/>
											Incluir termo
									</a>
								</td>
							</tr>
						</tfoot>
					<?php endif; ?>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subtituloCentro">Cursos/Abas</td>
		</tr>
		<tr>
			<td colspan="2">
				<?php 
				
				if($_REQUEST['prgid']){
					$sql = "select 
								pfc.pfcid,
								pfc.pfcdescricao,
								pfc.pfcorientacoes,
								fun.fundsc 
							from 
								par.pfcurso pfc
							left join 
								entidade.funcao fun on fun.funid = pfc.funid 
							where 
								prgid = {$_REQUEST['prgid']}
							and 
								pfc.pfcstatus = 'A'";
					
					$rsCursos = $db->carregar($sql);
				}
				$rsCursos = $rsCursos ? $rsCursos : array();
				?>
				<table id="tb_curso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<th>Ações</th>
							<th>Descrição</th>
							<th>Função</th>
							<th>Possui orientações</th>
						</tr>
					</thead>
					<tbody>
						<?php $x=0; ?>
						<?php foreach($rsCursos as $curso): ?>
							<?php $cor = $x % 2 ? '#f0f0f0' : 'white'; ?>
							<tr style="background:<?php echo $cor; ?>">
								<td align="center" width="80">
									<img src="../imagens/alterar.gif" style="cursor:pointer" alt="Editar" title="Editar" onclick="modalCurso(<?php echo $curso['pfcid']?>)" />
								</td>
								<td><?php echo $curso['pfcdescricao']; ?></td>
								<td><?php echo $curso['fundsc']; ?></td>
								<td align="center" width="200"><?php echo !empty($curso['pfcorientacoes']) ? '<img src="../imagens/check_p.gif" />' : '<img src="../imagens/exclui_p.gif" />'; ?></td>
							</tr>
						<?php endforeach; ?>						
					</tbody>
					<?php if($prgid): ?>
						<tfoot>
							<tr>
								<td colspan="4" style="border-top: 0px solid black;">
									<a href="javascript:void(0)" onclick="modalCurso('')">
										<img src="../imagens/gif_inclui.gif" align="absmiddle"/>
											Incluir curso/aba
									</a>
								</td>
							</tr>
						</tfoot>
					<?php endif; ?>
				</table>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Salvar" id="btnSalvar" />
			</td>
		</tr>
	</table>
</form>
<style>
#icone_programa{
	position: absolute;
	right: 110px;
	top: 215px;
	padding:5px;
	border: 1px solid black;
	background: white;
	cursor:pointer;	
}
#div_btn_alterar_foto{
	padding: 5px;
	margin-top: -25px;	
}
.alterar_foto{
	
}
</style>
<script>
	jQuery(function(){
		jQuery('#icone_programa').mouseover(function(){		
			jQuery('#div_btn_alterar_foto').show();
		});
		jQuery('#icone_programa').mouseout(function(){		
			jQuery('#div_btn_alterar_foto').hide();
		});
		jQuery('.alterar_foto').click(function(){
			
			var prgid = jQuery('[name=prgid]').val(); 

			jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
			jQuery( '#dialog-confirm' ).dialog({
				resizable: true,
				width: 800,				
				modal: true,
				open: function(event, ui){
				
					var url = 'par.php?modulo=sistema/tabelasapoio/feirao_programas/formPrograma&acao=A&popup=formFoto&prgid='+prgid;
					
					jQuery('#dialog-content').load(url,function(){
						
					});
				}
			});
		});		
	});
</script>
<?php if(($_REQUEST['prgid'] || $prgid) && $pfaid): ?>
	<div id="icone_programa">
		<?php $pfaicone = trim($pfaicone); ?>
		<?php if(!empty($pfaicone)): ?>
		
			<?php 
			$dirname = APPRAIZ."arquivos/proinfantil/feirao_programas/";
			$filename = $dirname.$pfaicone;
							
			if(!file_exists($filename)){
				$filename = '../imagens/'.$pfaicone;	
			}
			?>
			<center>
				<img height="80" src="<?php echo $filename; ?>" />
				<div  id="div_btn_alterar_foto" style="display:none;">
					<a href="javascript:void(0)" class="alterar_foto" style="background:#f0f0f0;padding:8px;padding-left:18;padding-right:18px;">Alterar imagem</a>
				</div>
			</center>
		<?php else: ?>
			<style>#icone_programa{ width: 142px; height: 105px; text-align: center; }</style>
			<br/><br/><br/>
			<a href="javascript:void(0)" class="alterar_foto">Adicionar imagem</a>
		<?php endif; ?>
	</div>
<?php endif; ?>
<div id="dialog-confirm" style="display:none;">		
	<div id="dialog-content">
	
	</div>		
</div>