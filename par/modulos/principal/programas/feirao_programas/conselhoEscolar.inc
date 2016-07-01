<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script type="text/javascript">	
	jQuery.noConflict();
</script>
<?php

if($_POST['arr']){
	
	$arrDados = explode("&",$_REQUEST['arr']);
	$flag = false;
	if($arrDados[0]) {
		foreach($arrDados as $arri) {
			$ar = explode("=",$arri);
			if($ar[1]) {
				$start = (stripos($ar[0],"%")+3);
				$end = strripos($ar[0],"%");
				
				$sql = "UPDATE par.pfcursista
						SET pscid = ".$ar[1]." 
						WHERE pcuid = ".substr($ar[0],$start,($end-$start));
				$db->executar($sql);
				$flag = true;
			}
		}
	}
	
	if($flag) {
		$db->commit();
		unset($_POST['arr']);
		die("<script>
				alert('Operação efetuada com sucesso.');
				location.href = 'par.php?modulo=principal/programas/feirao_programas/pfcursos&acao=A&pfcid=".$_REQUEST['pfcid']."&numero=".$_REQUEST['numero']."';
			</script>");
	}
 }

if($_REQUEST['pfcid']){
	
	$sql = "select 
				funid,
				pfctitulo,
				pfclimiteregistros,
				pfcorientacoes,
				pfcscripts,
				pfcidsformacao,
				pfcidsfuncaoatual
			from
				par.pfcurso
			where 
				pfcid = ".$_REQUEST['pfcid'];
	
	$rs = $db->pegaLinha($sql);
	if($rs) extract($rs);
}

if(!$_SESSION['par']['adpid']){
	echo '<script type="text/javascript"> 
    		alert("Sessão Expirou.\nFavor selecione o programa novamente!");
    		window.location.href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa";
    	  </script>';
    die;
}

//verifica se ja existe cursista na mesma escola
if($_POST['verificaCursista']){
	
	$sql = "SELECT pcuid FROM par.pfcursista 
			WHERE adpid = ".$_SESSION['par']['adpid']."
			AND pfcid = ".$_POST['pfcid']."
			AND entid = ".$_POST['escola'];
	$pcuidX = $db->pegaUm($sql);
			
	if($pcuidX){
		echo "ok";		
	}
	else{
		echo "";
	}
	die;
}

//pega array de perfis
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

//pega estado atual
$docid = pgCriarDocumento( $_SESSION['par']['adpid'] );
$esdid = prePegarEstadoAtual( $docid );

if( (in_array($esdid,array(WF_TIPO_PROFUNC_CADASTRAMENTO,WF_TIPO_PNAIC_CADASTRAMENTO)) || !$esdid) || (in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil)) || $_SESSION['par']['prgid'] == 157){
	
	require_once APPRAIZ . "includes/classes/entidades.class.inc";
	
	ini_set( "memory_limit", "1024M" );
	
	$obEntidade = new Entidades();
	$obPfCursista = new PfCursista();
	$obPfEntidade = new PfEntidade();
	
	$arPfCursista = $obPfCursista->recuperarPorAdpid($_SESSION['par']['adpid'], $_REQUEST['pcuid']);

	if ($_REQUEST['opt'] == 'excluir') {
	
		if($_REQUEST['pcuid']){
	
			$obPfCursista->excluir($_REQUEST['pcuid']);
			$obPfCursista->commit();
	
			echo "<script>
					alert('Dados excluidos com sucesso.');
					location.href = 'par.php?modulo=principal/programas/feirao_programas/pfcursos&acao=A&pfcid=".$_REQUEST['pfcid']."&numero=".$_REQUEST['numero']."';
				  </script>";
	
		}
		exit();
	}
	
	if ($_REQUEST['opt'] == 'salvarRegistro') {
		
		$pcucpf = str_replace(' ','',str_replace('/','',str_replace('-','',str_replace('.','',$_REQUEST['entnumcpfcnpj']))));
		$boEntid = $obPfCursista->verificaCursistaExiste($pcucpf, $_SESSION['par']['adpid'], FUNID_CURSISTA_PROGRAMA_FORMACAO, $_REQUEST['pfcid']);
		$boTutorCoordenador = $obPfCursista->verificaEntidadeCoordenadorTutorExiste($pcucpf, $_SESSION['par']['adpid']);
		
		if(!$boEntid && !$boTutorCoordenador || $_REQUEST['pcuid']){
			
			//ESCOLAS DE GESTORES
			if($_SESSION['par']['prgid'] == 12){
				
			 	//ESCOLAS DE GESTORES
				$sql = "SELECT pcuid FROM par.pfcursista 
						WHERE adpid = ".$_SESSION['par']['adpid']."
						AND pfcid = ".$_REQUEST['pfcid']."
						AND entid = ".$_REQUEST['escola'];
				$pcuidX = $db->pegaUm($sql);
				
				if($pcuidX){
					//delete
					$sql = "DELETE FROM par.pfcursista 
							WHERE pcuid = ".$pcuidX;
					$db->executar($sql);	
					$db->commit();	

					unset($_REQUEST['pcuid']); 
					
				}
			}
	
			$arPfcid[] = $_REQUEST['pfcid'];
			
			if(in_array($_SESSION['par']['prgid'], array(157))){
				$_REQUEST['escola'] = true;
			}
			
			if($pcucpf && $_REQUEST['entnome'] && $_REQUEST['entemail'] && $_REQUEST['escola']){
				
				if(!$_REQUEST['pffid']) $_REQUEST['pffid'] = 14;
				if(!$_REQUEST['tvpid']) $_REQUEST['tvpid'] = 4;
				
				$obPfCursista->pcuid 				= $_REQUEST['pcuid'] ? $_REQUEST['pcuid'] : ($_SESSION['par']['pcuid'] ? $_SESSION['par']['pcuid'] : null);
				$obPfCursista->adpid 				= $_SESSION['par']['adpid'];
				$obPfCursista->pfcid 				= $_REQUEST['pfcid'];
				$obPfCursista->entid 				= $_REQUEST['escola'];
				$obPfCursista->pcucpf 				= $pcucpf;
				$obPfCursista->pcunome 				= $_REQUEST['entnome'];
				$obPfCursista->tvpid 				= $_REQUEST['tvpid'];
				$obPfCursista->tfoid 				= $_REQUEST['tfoid'];
				$obPfCursista->pffid 				= $_REQUEST['pffid'];
				$obPfCursista->pcuemail 			= $_REQUEST['entemail'];
				$obPfCursista->pcudddnumtelefone 	= $_REQUEST['entnumdddcomercial'];
				$obPfCursista->pcunumtelefone 		= str_replace(' ','',str_replace('/','',str_replace('.','',str_replace('-','',$_REQUEST['entnumcomercial']))));
				$obPfCursista->pcufuncao 			= $_REQUEST['pffid'] == 14 ? $_REQUEST['pcufuncao'] : "";
				$obPfCursista->pcuvinculo 			= $_REQUEST['tvpid'] == 4 ? $_REQUEST['pcuvinculo'] : "";
				$obPfCursista->pcuano			 	= $_REQUEST['pcuano'];
				$obPfCursista->salvar();
				
				$obPfCursista->commit();
				
				$sql = "select pfcmsgsucesso from par.pfcurso where pfcid = {$_REQUEST['pfcid']}";
				$pfcmsgsucesso = $db->pegaUm($sql);
				
				$msg = 'Dados gravados com sucesso.';
				if($pfcmsgsucesso){
					$msg = $pfcmsgsucesso;
				}
				
				echo "<style> .ui-dialog-buttonset, button{ font-size:12px; } .ui-dialog-titlebar{ font-size:14px; } .ui-dialog-titlebar-close{ display: none; }</style>";
				echo '<div id="dialog-confirm" title="Confirmação" style="display:none;font-size:12px;">
						<div id="dialog-content"></div>
					  </div>';				
				echo "<script>										
						jQuery(function(){
							jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});				
							jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
							jQuery( '#dialog-content' ).html('{$msg}');							
							jQuery( '#dialog-confirm' ).dialog({
								resizable: false,
								width: 550,
								modal: true,
								buttons: {
									'Ok': function() {									
										jQuery( this ).dialog( 'close' );
										window.location.href='par.php?modulo=principal/programas/feirao_programas/pfcursos&acao=A&pfcid={$_REQUEST['pfcid']}';
									}
								}
							});
						});										
				  </script>";
				exit;
			
			}else{
				
				echo "<script>
					alert('Preencha todos os campos.');
					document.location.href = 'par.php?modulo=principal/programas/feirao_programas/pfcursos&acao=A&pfcid={$_REQUEST['pfcid']}';
				  </script>";
				exit;
			}
	
		}else{
	
			$stTipo = $boTutorCoordenador['funid'] == 87 ? "coordenador" : "professor tutor";
	
			echo "<script>
					alert('".($boTutorCoordenador['entid'] ? 'Este CPF pertence a um '.$stTipo.'.' : 'Cursista já cadastrado.')."');
					document.location.href = 'par.php?modulo=principal/programas/feirao_programas/pfcursos&acao=A&pfcid={$_REQUEST['pfcid']}';
				  </script>";
			exit;
	
		}
	}
}

include_once "termoprofuncionario.inc";

$aderiu = testaAdesaoProFuncionario();
if( $aderiu == 'S' ){
include_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br>';
echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/pfcursos&acao=A&pfcid='.$_REQUEST['pfcid']);

// Verifica se o titulo foi preenchido na tabela de cursos
if(!empty($pfctitulo)){
	$titulo_modulo = $pfctitulo; 
}

$titulo_modulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo( $titulo_modulo, $titulo_modulo2);

if($pfcorientacoes){
	echo '<table cellspacing="1" cellpadding="15" bgcolor="#f5f5f5" align="center" id="tblentidade" class="tabela">
			<tr>
				<td>'.$pfcorientacoes.'</td>
				<td width="150px;">&nbsp;</td>
			</tr>
		  </table>';	
}

//calculo de vagas
if($_SESSION['par']['prgid'] == 12){//ESCOLAS DE GESTORES
	
	if($_SESSION['par']['itrid'] == 2){//MUNICIPIO
		
		$sql = "select munpopulacao from territorios.municipio  
				WHERE muncod = '{$_SESSION['par']['muncod']}'";
		
		$totalPopulacao = $db->pegaUm($sql);
		
		if(!$totalPopulacao) $totalPopulacao = 0;

		if($totalPopulacao <= 10000){
			$totalVagas = 1;
		}
		elseif($totalPopulacao <= 50000){
			$totalVagas = 3;
		}
		elseif($totalPopulacao <= 100000){
			$totalVagas = 8;
		}
		elseif($totalPopulacao <= 500000){
			$totalVagas = 15;
		}
		elseif($totalPopulacao > 500000){
			$totalVagas = 30;
		}
		
	}else{//ESTADO
		
		$sql = "SELECT count(ent.entid) as total
			  FROM entidade.entidade ent
			  INNER JOIN entidade.endereco eed ON ent.entid = eed.entid and eed.estuf = '{$_SESSION['par']['estuf']}'
			  INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = 3 
			  WHERE	ent.entstatus = 'A'
			  AND ent.tpcid = 1";
		
		$total = $db->pegaUm($sql);
		if(!$total) $total = 0;
		$totalVagas = ($total * 15) / 100;
		
	}
	
	$sql = "SELECT count(pcuid) as total
	   		FROM par.pfcursista 
	   		WHERE adpid = {$_SESSION['par']['adpid']}
	   		AND pfcid = {$_REQUEST['pfcid']}";
	
	$totalCursista = $db->pegaUm($sql);
	
	if(!$totalCursista) $totalCursista = 0;

	$totalVagasInseridas = $totalVagas;
	if( in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ){
		$totalVagasInseridas = $totalVagas*2;
	}
	
	$saldoVagas = $totalVagasInseridas - $totalCursista;
	
}elseif($_SESSION['par']['prgid'] == 9){//PRADIME

	$totalVagas = 2;
	
	$sql = "SELECT count(pcuid) as total
	   		FROM par.pfcursista 
	   		WHERE adpid = {$_SESSION['par']['adpid']}
	   		AND pfcid = {$_REQUEST['pfcid']}";
	$totalCursista = $db->pegaUm($sql);
	if(!$totalCursista) $totalCursista = 0;

	$totalVagasInseridas = $totalVagas;
	
	$saldoVagas = $totalVagasInseridas - $totalCursista;
}
//fim calculo de vagas
?>
<?php if( (in_array($esdid,array(WF_TIPO_PROFUNC_CADASTRAMENTO,WF_TIPO_PNAIC_CADASTRAMENTO)) || !$esdid) || (in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil)) || $_SESSION['par']['prgid'] == 157){ ?>	
	<script type="text/javascript">
	<!--	
	jQuery(document).ready(function(){

		jQuery('#frmEntidade').submit(function(){
			jQuery('#btngravar').val('Aguarde...').attr('disabled', true);
		});
		
		jQuery('#btngravar').val('Salvar');
		jQuery('#btncancelar').val('Limpar');
	
		jQuery('#btncancelar').click(function(){
			jQuery('#frmEntidade').each(function(){
				for(x=0;x<this.length;x++){
					if(this[x].type != 'button' &&
					   this[x].type != 'submit' &&
					   this[x].id   != 'funid' &&
					   this[x].id   != 'entidassociado'){
	
						this[x].value = '';
					}
				}
			});
		});
	
		jQuery('select[name=pffid]').live('change', function(){
			if(this.value == 14){
				jQuery('#pcufuncao').show();
				jQuery('#opcfuncao').show();
				this.hide();
			}
		});
	
		jQuery('select[name=tvpid]').live('change', function(){
			if(this.value == 4){
				jQuery('#pcuvinculo').show();
				jQuery('#opcvinculo').show();
				this.hide();
			}
		});
	
		jQuery('.mostraOpcoes').live('click', function(){
	
			if(this.id == 'opcfuncao'){
	
				this.hide();
				jQuery('#pcufuncao').hide();
				jQuery('select[name=pffid]').show();
				jQuery('select[name=pffid]').val('');
	
			} else if(this.id == 'opcvinculo'){
	
				this.hide();
				jQuery('#pcuvinculo').hide();
				jQuery('select[name=tvpid]').show();
				jQuery('select[name=tvpid]').val('');
			}
		});
	
		var cpFormacao 	 = jQuery('#campoFormacao');
		var cpVinculo 	 = jQuery('#campoVinculo');
		var cpFuncao 	 = jQuery('#campoFuncao');
		var campoEscolas = jQuery('#campoEscolas');
		<?if($_SESSION['par']['prgid'] == 12){//ESCOLAS DE GESTORES?>
			var campoAno 	 = jQuery('#campoAno');
		<?}?>
	
		jQuery('#tr_entnumdddcomercial').find('td:first').html('Telefone :');
		jQuery('#tr_entobs').after('<tr id="tr_formacao"><td class="SubTituloDireita" width="30%">Formação (Escolaridade):</td><td>'+cpFormacao.html()+'</td></tr>');
		jQuery('#tr_formacao').after('<tr id="tr_vinculo"><td class="SubTituloDireita">Vinculo :</td><td>'+cpVinculo.html()+'</td></tr>');
		jQuery('#tr_vinculo').after('<tr id="tr_funcao"><td class="SubTituloDireita">Função atual:</td><td>'+cpFuncao.html()+'</td></tr>');
		jQuery('#tr_funcao').after('<tr id="tr_escolas"><td class="SubTituloDireita">Escolas :</td><td>'+campoEscolas.html()+'</td></tr>');
		<?if($_SESSION['par']['prgid'] == 12){//ESCOLAS DE GESTORES?>
			jQuery('#tr_escolas').after('<tr id="tr_ano"><td class="SubTituloDireita">Ano da demanda :</td><td>'+campoAno.html()+'</td></tr>');
		<?}?>
		jQuery('#tr_acoes').after('<tr id="tr_hidden"><td colspan="2"><input type="hidden" name="pcuid" value="<?php echo $arPfCursista['pcuid'] ?>" /></td></tr>');
		//jQuery('#tr_acoes').after('<tr id="tr_hidden" style="display:none;"><td colspan="2"><select name="pcuid"><option value="<?php echo $arPfCursista['pcuid'] ?>">pcuid</option></select></td></tr>');
	
		cpFormacao.remove();
		cpVinculo.remove();
		cpFuncao.remove();
		campoEscolas.remove();
		<?if($_SESSION['par']['prgid'] == 12){//ESCOLAS DE GESTORES?>
			campoAno.remove();
		<?}?>
	
		jQuery('#tr_funcoescadastradas').hide();
		jQuery('#tr_entcodent').hide();
		jQuery('#tr_entunicod').hide();
		jQuery('#tr_entungcod').hide();
		jQuery('#tr_njuid').hide();
		jQuery('#tr_tpctgid').hide();
		jQuery('#tr_tpcid').hide();
		jQuery('#tr_tplid').hide();
		jQuery('#tr_tpsid').hide();
	
		jQuery('#tr_entnumdddresidencial').hide();
		jQuery('#tr_entdatanasc').hide();
		jQuery('#tr_entorgaoexpedidor').hide();
		jQuery('#tr_entnumrg').hide();
		jQuery('#tr_entsexo').hide();
		jQuery('#tr_entnumdddfax').hide();
		jQuery('#tr_entnumdddcelular').hide();
	
		jQuery('#tr_endereco').hide();
		jQuery('#tr_endcom').hide();
		jQuery('#tr_endnum').hide();
		jQuery('#tr_endbai').hide();
		jQuery('#tr_estuf').hide();
		jQuery('#tr_mundescricao').hide();
		jQuery('#tr_latitude').hide();
		jQuery('#tr_longitude').hide();
		jQuery('#tr_endmapa').hide();
	
		jQuery('#tr_endlog').hide();
		jQuery('#tr_endcep').hide();
		jQuery('#tr_funid').hide();
		jQuery('#tr_entobs').hide();
	
		jQuery('input[name=entnumrg]').attr('disabled', 			true);
		jQuery('input[name=entorgaoexpedidor]').attr('disabled', 	true);
		jQuery('input[name=entnumdddresidencial]').attr('disabled', true);
		jQuery('input[name=entdatanasc]').attr('disabled', 			true);
		jQuery('input[name=entnumresidencial]').attr('disabled', 	true);
		jQuery('input[name=entnumdddfax]').attr('disabled', 		true);
		jQuery('input[name=entnumfax]').attr('disabled', 			true);
		jQuery('input[name=entnumramalfax]').attr('disabled', 		true);
		jQuery('input[name=entnumdddcelular]').attr('disabled', 	true);
		jQuery('input[name=entnumcelular]').attr('disabled', 		true);
		jQuery('input[name=mundescricao1]').attr('disabled', 		true);
	
		jQuery('#endid1').attr('disabled', 							true);
		jQuery('#endcom1').attr('disabled', 						true);
		jQuery('#endnum1').attr('disabled', 						true);
		jQuery('#endbai1').attr('disabled', 						true);
		jQuery('#estuf1').attr('disabled', 							true);
		jQuery('#mundescricao1').attr('disabled', 					true);
		jQuery('#muncod1').attr('disabled', 						true);
		jQuery('#graulatitude1').attr('disabled', 					true);
		jQuery('#minlatitude1').attr('disabled', 					true);
		jQuery('#seglatitude1').attr('disabled', 					true);
		jQuery('#pololatitude1').attr('disabled', 					true);
		jQuery('#graulongitude1').attr('disabled', 					true);
		jQuery('#minlongitude1').attr('disabled', 					true);
		jQuery('#seglongitude1').attr('disabled', 					true);
		jQuery('#pololongitude1').attr('disabled', 					true);
		jQuery('#endzoom1').attr('disabled', 						true);
	
		jQuery('#entsexo').attr('disabled', 						true);

		<?if( !in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) && !in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ){?>
			jQuery('#entnome').attr('disabled', true);
		<?}?>
	
		jQuery('#entnumcomercial').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
		jQuery('#entemail').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
		jQuery('#entnome').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
		jQuery('#entnumcpfcnpj').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	
		jQuery('#frmEntidade').submit(function(){
	
			if(jQuery('#tr_entnumcpfcnpj').css('display') != 'none'){
				if(jQuery('#entnumcpfcnpj').val()  == ''){
					alert('O campo CPF é obrigatório.');
					jQuery('#entnumcpfcnpj').focus();
					return false;
				}
			}
	
			if(jQuery('#tr_entnome').css('display') != 'none'){
				if(jQuery('#entnome').val()  == ''){
					alert('O campo Nome é obrigatório.');
					jQuery('#entnome').focus();
					return false;
				}
			}
	
			if(jQuery('#tr_entemail').css('display') != 'none'){
				if(jQuery('#entemail').val()  == ''){
					alert('O campo E-mail é obrigatório.');
					jQuery('#entemail').focus();
					return false;
				}
			}
			
			if(jQuery('#tr_entemail').css('display') != 'none'){
				var cemail = jQuery('#entemail').val();
				if(cemail.indexOf('@') == -1){
					alert('E-mail inválido.');
					jQuery('#entemail').focus();
					return false;
				}
			}

			if(jQuery('#tr_entnumdddcomercial').css('display') != 'none'){
				if(jQuery('#entnumdddcomercial').val()  == ''){
					alert('O campo DDD é obrigatório.');
					jQuery('#entnumdddcomercial').focus();
					return false;
				}
	
				if(jQuery('#entnumcomercial').val()  == ''){
					alert('O campo Telefone é obrigatório.');
					jQuery('#entnumcomercial').focus();
					return false;
				}
			}
	
			if(jQuery('select[name=tfoid]').val()  == ''){
				alert('O campo Formação é obrigatório.');
				jQuery('select[name=tfoid]').focus();
				return false;
			}
	
			if(jQuery('select[name=tvpid]').css('display') != 'none'){
				if(jQuery('select[name=tvpid]').val()  == ''){
					alert('O campo Vínculo é obrigatório.');
					jQuery('select[name=tvpid]').focus();
					return false;
				}
			}
	
			if(jQuery('#pcuvinculo').css('display') != 'none'){
				if(jQuery('#pcuvinculo').val()  == ''){
					alert('O campo Vínculo é obrigatório.');
					jQuery('#pcuvinculo').focus();
					return false;
				}
			}
	
			if(jQuery('select[name=pffid]').css('display') != 'none'){
				if(jQuery('select[name=pffid]').val()  == ''){
					alert('O campo Função é obrigatório.');
					jQuery('select[name=pffid]').focus();
					return false;
				}
			}
	
			if(jQuery('#pcufuncao').css('display') != 'none'){
				if(jQuery('#pcufuncao').val()  == ''){
					alert('O campo Função é obrigatório.');
					jQuery('#pcufuncao').focus();
					return false;
				}
			}

			if(jQuery('select[name=escola]').val() == ''){
				alert('O campo Escola é obrigatório.');
				jQuery('select[name=escola]').focus();
				return false;
			}
			
			<?php if($_SESSION['par']['prgid'] == 12){//ESCOLAS DE GESTORES?>
			if(jQuery('select[name=pcuano]').val() == ''){
				alert('O campo Ano da demanda é obrigatório.');
				jQuery('select[name=pcuano]').focus();
				return false;
			}

			var existe = false;
			// verifica se o cursista ja existe na mesma escola
			jQuery.ajax({type: "POST",
			   		url: "par.php?modulo=principal/programas/feirao_programas/pfcursos&acao=A",
			   		data: "verificaCursista=1&pfcid=<?=$_REQUEST['pfcid']?>&escola="+jQuery('select[name=escola]').val(),
			   		async: false,
			   		success: function(msg){
		   				if(msg){
		   					if(!confirm('Já existe um profissional inscrito nesta mesma escola.\n Deseja substituí-lo?')){
		   						existe = true;
		   						return false;
							}
		   				}

			   		}
			 });

			if(existe){
				return false;
			}
			<?php }?>
			
			jQuery('#entnome').attr('disabled', false);
			
		});
	
		if(jQuery('#tr_entnumcomercial').css('display') != 'none'){
			jQuery('#entnumcomercial').blur(function(){
				numero = this.value.replace('-','');
				if(numero.length < 8 && numero != ''){
					alert('Telefone inválido.');
					jQuery('#entnumcomercial').val('');
					jQuery('#entnumcomercial').focus();
					return false;
				}
			});
	
			jQuery('#entnumdddcomercial').blur(function(){
				numero = this.value.replace('-','');
				if(numero.length < 2 && numero != ''){
					alert('DDD inválido.');
					jQuery('#entnumdddcomercial').val('');
					jQuery('#entnumdddcomercial').focus();
					return false;
				}
			});
		}
	
		jQuery('.alterar').click(function(){
			var pfcid = "<?=$_REQUEST['pfcid']?>";
			document.location.href = '?modulo=principal/programas/feirao_programas/pfcursos&acao=A&pfcid='+pfcid+'&pcuid='+this.id;
		});
	
		jQuery('.excluir').click(function(){
			if(confirm('Deseja realmente excluir este registro?')){
				var numero = "<?=$_REQUEST['numero']?>";
				var pfcid = "<?=$_REQUEST['pfcid']?>";
				document.location.href = '?modulo=principal/programas/feirao_programas/pfcursos&acao=A&numero='+numero+'&pfcid='+pfcid+'&opt=excluir&pcuid='+this.id;
			}
		});
	
		<?php if($_REQUEST['pcuid']):?>
			jQuery('#entnumcpfcnpj').val("<?php echo $arPfCursista['pcucpf']?>");
			jQuery('#entnome').val("<?php echo $arPfCursista['pcunome']?>");
			jQuery('#entemail').val("<?php echo $arPfCursista['pcuemail']?>");
			jQuery('#entnumdddcomercial').val("<?php echo $arPfCursista['pcudddnumtelefone']?>");
			jQuery('#entnumcomercial').val("<?php echo $arPfCursista['pcunumtelefone']?>");
		<?php endif;?>
	
		<?php if($_REQUEST['pfcid'] == CURSO_FORMACAO_PEDAGOGICA): ?>
			jQuery('#tblentidade').remove();
			jQuery('.listagem:first').find('tr').find('td:first').remove();
		<?php endif; ?>

		jQuery('#tr_acoes').attr({'height':'80px','valign':'middle'});
	
	});

	</script>	
	<?php
	
	if(empty($funid)){
		$funid = FUNID_CURSISTA_PROGRAMA_FORMACAO;
	}
	
	$funentassoc = $funentassoc ? $funentassoc : null;
		
	echo $obEntidade->formEntidade("par.php?modulo=principal/programas/feirao_programas/pfcursos&acao=A&pfcid={$_REQUEST['pfcid']}&opt=salvarRegistro",
								 array("funid" => $funid, "entidassociado" => $funentassoc),
								 array( "enderecos"=>array(1) ) );
								 
	if($pfclimiteregistros == 1){
		
		$sql = "select
					*
				from
					par.pfcursista
				where
					pfcid = ".$_REQUEST['pfcid']."
				and
					adpid = ".$_SESSION['par']['adpid'];
		
		$rsCursista = $db->pegaLinha($sql);
		
		if($rsCursista){
			$rsCursista['pcucpf'] = formatar_cpf($rsCursista['pcucpf']);
			
			unset($_SESSION['par']['pcuid']);
			if($rsCursista['pcuid']){
				$_SESSION['par']['pcuid'] = $rsCursista['pcuid'];
			}
			echo "<script>
					jQuery(function(){
						jQuery('[name=entemail]').val('{$rsCursista['pcuemail']}');
						jQuery('[name=entnumdddcomercial]').val('{$rsCursista['pcudddnumtelefone']}');
						jQuery('[name=entnumcomercial]').val('{$rsCursista['pcunumtelefone']}');
						jQuery('[name=entnome]').val('{$rsCursista['pcunome']}');
						jQuery('[name=entnumcpfcnpj]').val('{$rsCursista['pcucpf']}');
						jQuery('[name=tfoid]').val('{$rsCursista['tfoid']}');						
						jQuery('[name=tvpid]').val('{$rsCursista['tvpid']}');
						jQuery('[name=pffid]').val('{$rsCursista['pffid']}');
						jQuery('[name=pcuid]').val('{$rsCursista['pcuid']}');
						jQuery('[name=escola]').val('{$rsCursista['entid']}');						
					});
				  </script>";
		}
		
		if($pfcscripts){
			echo $pfcscripts;
		}
	}
	
	$dados_wf = array('adpid' => $_SESSION['par']['adpid']);
	
	echo '<div style="position: absolute;top:315px; right: 80px;">';
	wf_desenhaBarraNavegacao($docid, $dados_wf);
	echo '</div>';
	
	
	if($_SESSION['par']['prgid'] == 12 || $_SESSION['par']['prgid'] == 9){//12 = ESCOLAS DE GESTORES / 9 = PRADIME
		
		echo '<div style="position: absolute;top:315px; right: 170px;">';					
		echo '<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total geral de vagas">
							<b>Total geral de vagas</b>
						</span>
					</td>
				</tr>
				<tr style="text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total geral de vagas">
							'.$totalVagas.'					
						</span>
					</td>
				</tr>
				<tr style="background-color: #c9c9c9; text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total de inscrições que podem ser inseridas">
							<b>Total de inscrições que podem ser inseridas</b>
						</span>
					</td>
				</tr>
				<tr style="text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total de inscrições que podem ser inseridas">
							'.$totalVagasInseridas.'					
						</span>
					</td>
				</tr>
				<tr style="background-color: #c9c9c9; text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total de inscrições inseridas">
							<b>Total de inscrições inseridas</b>
						</span>
					</td>
				</tr>
				<tr style="text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total de inscrições inseridas">
							'.$totalCursista.'					
						</span>
					</td>
				</tr>
				<tr style="background-color: #c9c9c9; text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Saldo de inscrições">
							<b>Saldo de inscrições</b>
						</span>
					</td>
				</tr>
				<tr style="text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Saldo de inscrições">
							'.$saldoVagas.'					
						</span>
					</td>
				</tr>
			</table><br>';
		
		echo '</div>';
		
		echo '<div style="position: absolute;top:315px; right: 260px;">';
		if($saldoVagas == 0){
			echo '<table border="0" cellpadding="3" cellspacing="0" style="width: 80px;">
					<tr style="text-align:center;">
						<td style="font-size:7pt; text-align:center;">
							<span title="Total geral de vagas">
								<font color=blue>Foram preenchidas todas as vagas</font>				
							</span>
						</td>
					</tr>
					</table>';
		}else{
			echo '<table border="0" cellpadding="3" cellspacing="0" style="width: 80px;">
					<tr style="text-align:center;">
						<td style="font-size:7pt; text-align:center;">
							<span title="Total geral de vagas">
								<font color=red>Não foram preenchidas todas as vagas</font>				
							</span>
						</td>
					</tr>
					</table>';
		}
		echo '</div>';
	}	
	
	
}else{
	
	echo "<table border=0 cellpading=0 cellspacing=0 width='100%'>
			<tr><td width='90%'>&nbsp;";
	echo "</td>";
	echo "<td width='10%'>";	
	
	$dados_wf = array('adpid' => $_SESSION['par']['adpid']);
	wf_desenhaBarraNavegacao($docid, $dados_wf);
	
	if($_SESSION['par']['prgid'] == 12 || $_SESSION['par']['prgid'] == 9){//12 = ESCOLAS DE GESTORES / 9 = PRADIME
		echo '<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total geral de vagas">
							<b>Total geral de vagas</b>
						</span>
					</td>
				</tr>
				<tr style="text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total geral de vagas">
							'.$totalVagas.'					
						</span>
					</td>
				</tr>
				<tr style="background-color: #c9c9c9; text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total de inscrições que podem ser inseridas">
							<b>Total de inscrições que podem ser inseridas</b>
						</span>
					</td>
				</tr>
				<tr style="text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total de inscrições que podem ser inseridas">
							'.$totalVagasInseridas.'					
						</span>
					</td>
				</tr>
				<tr style="background-color: #c9c9c9; text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total de inscrições inseridas">
							<b>Total de inscrições inseridas</b>
						</span>
					</td>
				</tr>
				<tr style="text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Total de inscrições inseridas">
							'.$totalCursista.'					
						</span>
					</td>
				</tr>
				<tr style="background-color: #c9c9c9; text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Saldo de inscrições">
							<b>Saldo de inscrições</b>
						</span>
					</td>
				</tr>
				<tr style="text-align:center;">
					<td style="font-size:7pt; text-align:center;">
						<span title="Saldo de inscrições">
							'.$saldoVagas.'					
						</span>
					</td>
				</tr>
			</table><br>';
	}	
	
	echo "</td></tr></table>";

}	

echo "<form name='formulario' method='post' action=>";
echo "<input type=hidden name=arr id=arr>";
echo "<input type=hidden name=numero id=numero>";

$permissaoPreAnalise = 0;
if( in_array(PAR_PERFIL_PROFUNC_PREANALISEPF,$arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ){
	$permissaoPreAnalise = 1;
}

$permissaoAnalise = 0;
if( in_array(PAR_PERFIL_PROFUNC_ANALISEPF,$arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ){
	$permissaoAnalise = 1;
}

$botoes = "'<img src=\"../imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\" class=\"excluir\" id=\"' || pcu.pcuid || '\" style=\"cursor:pointer\" />
  			<img src=\"../imagens/alterar.gif\" alt=\"Alterar\" title=\"Alterar\" class=\"alterar\" id=\"' || pcu.pcuid || '\" style=\"cursor:pointer\"/>' as acao,";

if( (in_array($esdid,array(WF_TIPO_PROFUNC_CADASTRAMENTO,WF_TIPO_PNAIC_CADASTRAMENTO)) || !$esdid) ){

	if($_REQUEST['pfcid'] != 5){
	
		if($_SESSION['par']['prgid'] == 12){//12 = ESCOLAS DE GESTORES
			
		    $sql = "SELECT DISTINCT
		    			'<img src=\"../imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\" class=\"excluir\" id=\"' || pcu.pcuid || '\" style=\"cursor:pointer\" />
		    			 <img src=\"../imagens/alterar.gif\" alt=\"Alterar\" title=\"Alterar\" class=\"alterar\" id=\"' || pcu.pcuid || '\" style=\"cursor:pointer\"/>' as acao,
		    			pcu.pcucpf,
		    			pcu.pcunome,
		    			pcu.pcuemail,
		    			'(' || pcu.pcudddnumtelefone || ') ' || pcu.pcunumtelefone as telefone,
		    			tfo.tfodsc,
		    			case when pcu.tvpid = 4 then pcu.pcuvinculo
		    			  else pfv.tvpdsc
		    			end as tvpdsc,
		    			case when pcu.pffid = 14 then pcu.pcufuncao
		    			 else pff.pffdescricao
		    			end as pffdescricao,
		    			--pfv.tvpdsc,
		    			--pff.pffdescricao,
		    			ent.entnome as escola,
		    			pcu.pcuano
		    		FROM par.pfcursista pcu
		    			INNER JOIN public.tipovinculoprofissional	pfv ON pfv.tvpid = pcu.tvpid
		    			INNER JOIN par.pffuncao						pff ON pff.pffid = pcu.pffid
		    			INNER JOIN public.tipoformacao				tfo ON tfo.tfoid = pcu.tfoid
		    			INNER JOIN entidade.entidade 				ent ON ent.entid = pcu.entid
		    			INNER JOIN entidade.endereco 				eed ON eed.entid = ent.entid
		    			LEFT JOIN par.pfsituacaocursista			pfc ON pfc.pscid = pcu.pscid
		    		WHERE pcu.adpid = {$_SESSION['par']['adpid']}
		    		AND pfcid = {$_REQUEST['pfcid']}
		    		order by 3";

		    $cabecalho = array( "Ação", "CPF", "Nome", "Email", "Telefone", "Formação (Escolaridade)", "Vínculo", "Função atual", "Escola", "Ano da demanda");
		    
		}else{
			
		    $sql = "SELECT DISTINCT
		    			'<img src=\"../imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\" class=\"excluir\" id=\"' || pcu.pcuid || '\" style=\"cursor:pointer\" />
		    			 <img src=\"../imagens/alterar.gif\" alt=\"Alterar\" title=\"Alterar\" class=\"alterar\" id=\"' || pcu.pcuid || '\" style=\"cursor:pointer\"/>' as acao,
		    			pcu.pcucpf,
		    			pcu.pcunome,
		    			pcu.pcuemail,
		    			'(' || pcu.pcudddnumtelefone || ') ' || pcu.pcunumtelefone as telefone,
		    			tfo.tfodsc,
		    			case when pcu.tvpid = 4 then pcu.pcuvinculo
		    			  else pfv.tvpdsc
		    			end as tvpdsc,
		    			case when pcu.pffid = 14 then pcu.pcufuncao
		    			 else pff.pffdescricao
		    			end as pffdescricao,
		    			--pfv.tvpdsc,
		    			--pff.pffdescricao,
		    			ent.entnome as escola
		    		FROM par.pfcursista pcu
		    			INNER JOIN public.tipovinculoprofissional	pfv ON pfv.tvpid = pcu.tvpid
		    			INNER JOIN par.pffuncao						pff ON pff.pffid = pcu.pffid
		    			INNER JOIN public.tipoformacao				tfo ON tfo.tfoid = pcu.tfoid
		    			INNER JOIN entidade.entidade 				ent ON ent.entid = pcu.entid
		    			INNER JOIN entidade.endereco 				eed ON eed.entid = ent.entid
		    			LEFT JOIN par.pfsituacaocursista			pfc ON pfc.pscid = pcu.pscid
		    		WHERE pcu.adpid = {$_SESSION['par']['adpid']}
		    		AND pfcid = {$_REQUEST['pfcid']}
		    		order by 3";

		    $cabecalho = array( "Ação", "CPF", "Nome", "Email", "Telefone", "Formação", "Vínculo", "Função atual", "Escola");
		}
	
	}else{
	
	    $sql = "SELECT DISTINCT
	                '<img src=\"../imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\" class=\"excluir\" id=\"' || pcu.pcuid || '\" style=\"cursor:pointer\" />
	                 <img src=\"../imagens/alterar.gif\" alt=\"Alterar\" title=\"Alterar\" class=\"alterar\" id=\"' || pcu.pcuid || '\" style=\"cursor:pointer\"/>' as acao,
	                pcu.pcucpf,
	                pcu.pcunome,
	                pcu.pcuemail,
	                '(' || pcu.pcudddnumtelefone || ') ' || pcu.pcunumtelefone as telefone,
	                tfo.tfodsc,
	                case when pcu.tvpid = 4 then pcu.pcuvinculo
	                  else pfv.tvpdsc
	                end as tvpdsc,
	                case when pcu.pffid = 14 then pcu.pcufuncao
	                 else pff.pffdescricao
	                end as pffdescricao,
	                ent.entnome as escola,
	                pfc.pfcdescricao
	            FROM par.pfcursista pcu
	                INNER JOIN par.pfcurso                      pfc ON pfc.pfcid = pcu.pfcid
	                INNER JOIN public.tipovinculoprofissional   pfv ON pfv.tvpid = pcu.tvpid
	                INNER JOIN par.pffuncao                     pff ON pff.pffid = pcu.pffid
	                INNER JOIN public.tipoformacao              tfo ON tfo.tfoid = pcu.tfoid
	                INNER JOIN entidade.entidade                ent ON ent.entid = pcu.entid
	                INNER JOIN entidade.endereco                eed ON eed.entid = ent.entid
	            WHERE pcu.adpid = {$_SESSION['par']['adpid']}
	            order by 3";
	
	    $cabecalho = array("Ação", "CPF", "Nome", "Email", "Telefone", "Formação", "Vínculo", "Função", "Escola", "Curso");
	}
	
}else{
	
	if( !in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) && !in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ){
		$botoes = "";
	}
	
	if($_REQUEST['pfcid'] != 5){
		
		//monta combo query
		$sqlc = "select pscid, pscdescricao from par.pfsituacaocursista 
					where prgid = ".$_SESSION['par']['prgid']."
					and esdid = ".$esdid."
					and pscstatus = 'A'";
		$dadoscombo = $db->carregar($sqlc);
		if($dadoscombo){
			$combo = '';
			foreach($dadoscombo as $d){
				$combo .= '<option value='.$d['pscid'].'>'.$d['pscdescricao'].'</option>'; 
			}
		}
	
		if($_SESSION['par']['prgid'] == 12){//12 = ESCOLAS DE GESTORES
		    $sql = "SELECT DISTINCT
		    			$botoes
		    			pcu.pcucpf,
		    			pcu.pcunome,
		    			pcu.pcuemail,
		    			'(' || pcu.pcudddnumtelefone || ') ' || pcu.pcunumtelefone as telefone,
		    			tfo.tfodsc,
		    			case when pcu.tvpid = 4 then pcu.pcuvinculo
		    			  else pfv.tvpdsc
		    			end as tvpdsc,
		    			case when pcu.pffid = 14 then pcu.pcufuncao
		    			 else pff.pffdescricao
		    			end as pffdescricao,
		    			--pfv.tvpdsc,
		    			--pff.pffdescricao,
		    			ent.entnome as escola,
		    			pcu.pcuano,
		    			(CASE WHEN pscdescricao <> '' THEN
		    					pscdescricao
		    				  ELSE
		    				  	'Em análise'
		    			END) as sitatual,
		    			(CASE WHEN ".$esdid."=".WF_TIPO_PROFUNC_PREANALISE." OR ".$esdid."=".WF_TIPO_PROFUNC_ANALISE." OR ".$esdid."=".WF_TIPO_PROFUNC_MATRICULA." THEN
				    			'<select name=pcuidArray['||pcu.pcuid||']>' ||
				    			(CASE WHEN 1=$permissaoPreAnalise THEN
				    					(CASE WHEN pcu.pscid = 7 THEN
				    				  			'<option value=7 selected>Pré-selecionado</option><option value=8>Não selecionado</option>'
				    				  		  ELSE
				    				  			'<option value=7>Pré-selecionado</option><option value=8 selected>Não selecionado</option>'
				    				  	END)
				    				  ELSE
				    				  	'<option value=></option>'
				    		     END)
				    		     || '</select>' 
				    		  ELSE 
				    		  	''
				    	END) AS sitnova
		    		FROM par.pfcursista pcu
		    			INNER JOIN public.tipovinculoprofissional	pfv ON pfv.tvpid = pcu.tvpid
		    			INNER JOIN par.pffuncao						pff ON pff.pffid = pcu.pffid
		    			INNER JOIN public.tipoformacao				tfo ON tfo.tfoid = pcu.tfoid
		    			INNER JOIN entidade.entidade 				ent ON ent.entid = pcu.entid
		    			INNER JOIN entidade.endereco 				eed ON eed.entid = ent.entid
		    			LEFT JOIN par.pfsituacaocursista			pfc ON pfc.pscid = pcu.pscid
		    		WHERE pcu.adpid = {$_SESSION['par']['adpid']}
		    		AND pfcid = {$_REQUEST['pfcid']}
		    		order by 3";

			if(!$botoes){
		    	$cabecalho = array( "CPF", "Nome", "Email", "Telefone", "Formação (Escolaridade)", "Vínculo", "Função atual", "Escola", "Ano da demanda", "Situação atual", "Nova situação");
		    }else{
		    	$cabecalho = array("Ação", "CPF", "Nome", "Email", "Telefone", "Formação (Escolaridade)", "Vínculo", "Função atual", "Escola", "Ano da demanda", "Situação atual", "Nova situação");
		    }
		    
		}elseif($_SESSION['par']['prgid'] == 9){//9 = pradime
			
		    $sql = "SELECT DISTINCT
		    			$botoes
		    			pcu.pcucpf,
		    			pcu.pcunome,
		    			pcu.pcuemail,
		    			'(' || pcu.pcudddnumtelefone || ') ' || pcu.pcunumtelefone as telefone,
		    			tfo.tfodsc,
		    			case when pcu.tvpid = 4 then pcu.pcuvinculo
		    			  else pfv.tvpdsc
		    			end as tvpdsc,
		    			case when pcu.pffid = 14 then pcu.pcufuncao
		    			 else pff.pffdescricao
		    			end as pffdescricao,
		    			--pfv.tvpdsc,
		    			--pff.pffdescricao,
		    			ent.entnome as escola,
		    			--pcu.pcuano,
		    			(CASE WHEN pscdescricao <> '' THEN
		    					pscdescricao
		    				  ELSE
		    				  	'Em análise'
		    			END) as sitatual,
		    			(CASE WHEN ".$esdid."=".WF_TIPO_PROFUNC_PREANALISE." OR ".$esdid."=".WF_TIPO_PROFUNC_ANALISE." OR ".$esdid."=".WF_TIPO_PROFUNC_MATRICULA." THEN
				    			'<select name=pcuidArray['||pcu.pcuid||']>' ||
				    			(CASE WHEN 1=$permissaoPreAnalise THEN
				    					(CASE WHEN pcu.pscid = 11 THEN
				    				  			'<option value=11 selected>Pré-selecionado</option><option value=12>Não selecionado</option>'
				    				  		  ELSE
				    				  			'<option value=11>Pré-selecionado</option><option value=12 selected>Não selecionado</option>'
				    				  	END)
				    				  ELSE
				    				  	'<option value=></option>'
				    		     END)
				    		     || '</select>' 
				    		  ELSE 
				    		  	''
				    	END) AS sitnova
		    		FROM par.pfcursista pcu
		    			INNER JOIN public.tipovinculoprofissional	pfv ON pfv.tvpid = pcu.tvpid
		    			INNER JOIN par.pffuncao						pff ON pff.pffid = pcu.pffid
		    			INNER JOIN public.tipoformacao				tfo ON tfo.tfoid = pcu.tfoid
		    			INNER JOIN entidade.entidade 				ent ON ent.entid = pcu.entid
		    			INNER JOIN entidade.endereco 				eed ON eed.entid = ent.entid
		    			LEFT JOIN par.pfsituacaocursista			pfc ON pfc.pscid = pcu.pscid
		    		WHERE pcu.adpid = {$_SESSION['par']['adpid']}
		    		AND pfcid = {$_REQUEST['pfcid']}
		    		order by 3";
			
			if(!$botoes){
		    	$cabecalho = array( "CPF", "Nome", "Email", "Telefone", "Formação (Escolaridade)", "Vínculo", "Função atual", "Escola", "Situação atual", "Nova situação");
		    }else{
		    	$cabecalho = array("Ação", "CPF", "Nome", "Email", "Telefone", "Formação (Escolaridade)", "Vínculo", "Função atual", "Escola", "Situação atual", "Nova situação");
		    }
		    
		}elseif($_SESSION['par']['prgid'] == 60){ //PROFUNCIONARIO
			
		    $sql = "SELECT DISTINCT
		    			$botoes
		    			pcu.pcucpf,
		    			pcu.pcunome,
		    			pcu.pcuemail,
		    			'(' || pcu.pcudddnumtelefone || ') ' || pcu.pcunumtelefone as telefone,
		    			tfo.tfodsc,
		    			case when pcu.tvpid = 4 then pcu.pcuvinculo
		    			  else pfv.tvpdsc
		    			end as tvpdsc,
		    			case when pcu.pffid = 14 then pcu.pcufuncao
		    			 else pff.pffdescricao
		    			end as pffdescricao,
		    			--pfv.tvpdsc,
		    			--pff.pffdescricao,
		    			ent.entnome as escola,
		    			(CASE WHEN pscdescricao <> '' THEN
		    					pscdescricao
		    				  ELSE
		    				  	'Em análise'
		    			END) as sitatual,
		    			(CASE WHEN ".$esdid."=".WF_TIPO_PROFUNC_PREANALISE." OR ".$esdid."=".WF_TIPO_PROFUNC_ANALISE." OR ".$esdid."=".WF_TIPO_PROFUNC_MATRICULA." THEN
				    			'<select name=pcuidArray['||pcu.pcuid||']>' ||
				    			(CASE WHEN ".$esdid."=".WF_TIPO_PROFUNC_PREANALISE." AND 1=$permissaoPreAnalise THEN
				    					(CASE WHEN pcu.pscid = 1 THEN
				    				  			'<option value=1 selected>Pré-qualificado</option><option value=2>Não qualificado</option>'
				    				  		  ELSE
				    				  			'<option value=1>Pré-qualificado</option><option value=2 selected>Não qualificado</option>'
				    				  	END)
				    				  WHEN ".$esdid."=".WF_TIPO_PROFUNC_ANALISE." AND 1=$permissaoAnalise THEN
				    				  	(CASE WHEN pcu.pscid  = 3 THEN
				    				  			'<option value=3 selected>Qualificado para matrícula</option><option value=4>Não qualificado para matrícula</option>'
				    				  		  WHEN pcu.pscid in (1,4) THEN
				    				  			'<option value=3>Qualificado para matrícula</option><option value=4 selected>Não qualificado para matrícula</option>'	
				    				  		  ELSE
				    				  		  	'<option value=></option>'
				    				  	END)
				    				  WHEN ".$esdid."=".WF_TIPO_PROFUNC_MATRICULA." AND 1=$permissaoAnalise THEN
				    				  	(CASE WHEN pcu.pscid = 5 THEN
				    				  			'<option value=5 selected>Matriculado</option><option value=6>Não matrículado</option>'		
				    				  		  WHEN pcu.pscid in (3,6) THEN
				    				  			'<option value=5>Matriculado</option><option value=6 selected>Não matrículado</option>'
				    				  		  ELSE
				    				  		  	'<option value=></option>'
				    				  	END)
				    				  ELSE
				    				  	'<option value=></option>'
				    		     END)
				    		     || '</select>' 
				    		  ELSE 
				    		  	''
				    	END) AS sitnova
		    		FROM par.pfcursista pcu
		    			INNER JOIN public.tipovinculoprofissional	pfv ON pfv.tvpid = pcu.tvpid
		    			INNER JOIN par.pffuncao						pff ON pff.pffid = pcu.pffid
		    			INNER JOIN public.tipoformacao				tfo ON tfo.tfoid = pcu.tfoid
		    			INNER JOIN entidade.entidade 				ent ON ent.entid = pcu.entid
		    			INNER JOIN entidade.endereco 				eed ON eed.entid = ent.entid
		    			LEFT JOIN par.pfsituacaocursista			pfc ON pfc.pscid = pcu.pscid
		    		WHERE pcu.adpid = {$_SESSION['par']['adpid']}
		    		AND pfcid = {$_REQUEST['pfcid']}
		    		order by 3";
			//dbg($sql,1);
			if(!$botoes){
		    	$cabecalho = array( "CPF", "Nome", "Email", "Telefone", "Formação", "Vínculo", "Função", "Escola", "Situação Atual", "Nova Situação");
		    }else{
		    	$cabecalho = array("Ação", "CPF", "Nome", "Email", "Telefone", "Formação", "Vínculo", "Função", "Escola", "Situação Atual", "Nova Situação");
		    }
		}
		else{ //outros programas
				$sql = "SELECT DISTINCT
						$botoes
		    			pcu.pcucpf,
		    			pcu.pcunome,
		    			pcu.pcuemail,
		    			'(' || pcu.pcudddnumtelefone || ') ' || pcu.pcunumtelefone as telefone,
		    			tfo.tfodsc,
		    			case when pcu.tvpid = 4 then pcu.pcuvinculo
		    			  else pfv.tvpdsc
		    			end as tvpdsc,
		    			case when pcu.pffid = 14 then pcu.pcufuncao
		    			 else pff.pffdescricao
		    			end as pffdescricao,
		    			--pfv.tvpdsc,
		    			--pff.pffdescricao,
		    			ent.entnome as escola,
		    			(CASE WHEN pscdescricao <> '' THEN
		    					pscdescricao
		    				  ELSE
		    				  	'Em análise'
		    			END) as sitatual,
		    			(CASE WHEN ".$esdid."=".WF_TIPO_PROFUNC_PREANALISE." OR ".$esdid."=".WF_TIPO_PROFUNC_ANALISE." OR ".$esdid."=".WF_TIPO_PROFUNC_MATRICULA." THEN
				    			'<select name=pcuidArray['||pcu.pcuid||']>' ||
				    			(CASE WHEN 1=$permissaoPreAnalise THEN
				    					(CASE WHEN pcu.pscid = 9 THEN
				    				  			'<option value=9 selected>Pré-selecionado</option><option value=10>Não selecionado</option>'
				    				  		  ELSE
				    				  			'<option value=9>Pré-selecionado</option><option value=10 selected>Não selecionado</option>'
				    				  	END)
				    				  ELSE
				    				  	'<option value=></option>'
				    		     END)
				    		     || '</select>' 
				    		  ELSE 
				    		  	''
				    	END) AS sitnova
		    		FROM par.pfcursista pcu
		    			INNER JOIN public.tipovinculoprofissional	pfv ON pfv.tvpid = pcu.tvpid
		    			INNER JOIN par.pffuncao						pff ON pff.pffid = pcu.pffid
		    			INNER JOIN public.tipoformacao				tfo ON tfo.tfoid = pcu.tfoid
		    			INNER JOIN entidade.entidade 				ent ON ent.entid = pcu.entid
		    			INNER JOIN entidade.endereco 				eed ON eed.entid = ent.entid
		    			LEFT JOIN par.pfsituacaocursista			pfc ON pfc.pscid = pcu.pscid
		    		WHERE pcu.adpid = {$_SESSION['par']['adpid']}
		    		AND pfcid = {$_REQUEST['pfcid']}
		    		order by 3";

			if(!$botoes){
		    	$cabecalho = array( "CPF", "Nome", "Email", "Telefone", "Formação (Escolaridade)", "Vínculo", "Função atual", "Escola", "Situação atual", "Nova situação");
		    }else{
		    	$cabecalho = array("Ação", "CPF", "Nome", "Email", "Telefone", "Formação (Escolaridade)", "Vínculo", "Função atual", "Escola", "Situação atual", "Nova situação");
		    }
		}
	
	}else{
	
	    $sql = "SELECT DISTINCT
	    			$botoes
	                pcu.pcucpf,
	                pcu.pcunome,
	                pcu.pcuemail,
	                '(' || pcu.pcudddnumtelefone || ') ' || pcu.pcunumtelefone as telefone,
	                tfo.tfodsc,
	                case when pcu.tvpid = 4 then pcu.pcuvinculo
	                  else pfv.tvpdsc
	                end as tvpdsc,
	                case when pcu.pffid = 14 then pcu.pcufuncao
	                 else pff.pffdescricao
	                end as pffdescricao,
	                ent.entnome as escola,
	                pfc.pfcdescricao as curso,
	                (CASE WHEN pscdescricao <> '' THEN
	    					pscdescricao
	    				  ELSE
	    				  	'Em análise'
	    			END) as sitatual
	            FROM par.pfcursista pcu
	                INNER JOIN par.pfcurso                      pfc ON pfc.pfcid = pcu.pfcid
	                INNER JOIN public.tipovinculoprofissional   pfv ON pfv.tvpid = pcu.tvpid
	                INNER JOIN par.pffuncao                     pff ON pff.pffid = pcu.pffid
	                INNER JOIN public.tipoformacao              tfo ON tfo.tfoid = pcu.tfoid
	                INNER JOIN entidade.entidade                ent ON ent.entid = pcu.entid
	                INNER JOIN entidade.endereco                eed ON eed.entid = ent.entid
	                LEFT JOIN par.pfsituacaocursista			pfs ON pfs.pscid = pcu.pscid
	            WHERE pcu.adpid = {$_SESSION['par']['adpid']}
	            order by 3";
		
	    if(!$botoes){
	    	$cabecalho = array("CPF", "Nome", "Email", "Telefone", "Formação", "Vínculo", "Função", "Escola", "Curso", "Situação");
	    }else{
	    	$cabecalho = array("Ação","CPF", "Nome", "Email", "Telefone", "Formação", "Vínculo", "Função", "Escola", "Curso", "Situação");
	    }
	}
}

?>
</form>
<?php 
if($pfclimiteregistros > 1 || empty($pfclimiteregistros)){
	$db->monta_lista($sql,$cabecalho,50,5,'N','95%',"","formdados");
} 
?>
<center>
	<?if($esdid != WF_TIPO_PROFUNC_MATRICULA){		
		if( ($esdid == WF_TIPO_PROFUNC_PREANALISE || $esdid == WF_TIPO_PROFUNC_ANALISE || $esdid == WF_TIPO_PROFUNC_MATRICULA) &&
			($permissaoPreAnalise == 1 || $permissaoAnalise == 1) &&
			$_REQUEST['pfcid'] != 5 && $_REQUEST['pfcid'] != 12 ) {
	?>
		
		<br>
		<input type="button" value="Salvar" onclick="salvarSit();">
	<?}
	}?>	
</center>
<?
if( (in_array($esdid,array(WF_TIPO_PROFUNC_CADASTRAMENTO,WF_TIPO_PNAIC_CADASTRAMENTO)) || !$esdid) || (in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil)) || $_SESSION['par']['prgid'] == 157){
?>
	<div id="campoFormacao" style="display:none;">
		<?php
		
		$stWhere = '';
		if($pfcidsformacao){
			$stWhere = ' and tfoid in ('.$pfcidsformacao.') ';
		}
		
		$sql = "select
			tfoid as codigo,
			tfodsc as descricao
		from public.tipoformacao
		where tfostatus = 'A'
		{$stWhere}
		order by tfodsc";
		
		unset($stWhere);
	
		$db->monta_combo('tfoid', $sql, 'S', 'Selecione uma formação...', null, null, null, 200, 'S', '', '', $arPfCursista['tfoid']);		
		?>
	</div>
	<div id="campoVinculo" style="display:none;">
		<?php
		$sql = "select
					tvpid as codigo,
					tvpdsc as descricao
				from public.tipovinculoprofissional tvp
				where tvpstatus = 'A'
				order by tvpdsc";
	
		$db->monta_combo('tvpid', $sql, 'S', 'Selecione um vínculo...', null, null, null, 200, 'N', '', '', $arPfCursista['tvpid']);
		?>
		<input type="text" class="normal obrigatorio" id="pcuvinculo" name="pcuvinculo" style="width: 50ex; display: none;" value="<?php echo $arPfCursista['pcuvinculo']?>" />
		<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		<a href="javascript:void(0)" class="mostraOpcoes" id="opcvinculo" style="display:none;">Mostrar opções</a>
	</div>
	<div id="campoFuncao" style="display:none;">
		<?php
		
		$stWhere = '';
		if($pfcidsfuncaoatual){
			$stWhere = ' and pffid in ('.$pfcidsfuncaoatual.') ';
		}
		
		$sql = "select
					pffid as codigo,
					pffdescricao as descricao
				from par.pffuncao
				where pffstatus = 'A'
				{$stWhere}
				order by pffdescricao";
	
		unset($stWhere);
		
		$db->monta_combo('pffid', $sql, 'S', 'Selecione uma funcao...', null, null, null, 200, 'N', '', '', $arPfCursista['pffid']);
		?>
		<input type="text" class="normal obrigatorio" id="pcufuncao" name="pcufuncao" style="width: 50ex; display: none;" value="<?php echo $arPfCursista['pcufuncao']?>" />
		<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		<a href="javascript:void(0)" class="mostraOpcoes" style="display:none;" id="opcfuncao">Mostrar opções</a>
	</div>
	<div id="campoEscolas" style="display:none;">
		<?php
	
		if($_SESSION['par']['itrid'] == 2){
			$stWhere = "AND eed.muncod = '{$_SESSION['par']['muncod']}'";
		}else{
			$stWhere = "AND eed.estuf = '{$_SESSION['par']['estuf']}'";
		}
		
		if($_SESSION['par']['prgid'] == 12){//ESCOLAS DE GESTORES
			$sql_combo = "SELECT
							ent.entid as codigo,
							ent.entnome as descricao
						  FROM entidade.entidade ent
						  INNER JOIN entidade.endereco eed ON ent.entid = eed.entid {$stWhere}
						  -- INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = ".FUNID_ESCOLA."
						  WHERE	ent.entstatus = 'A'
						  AND ent.entcodent IS NOT NULL
						  AND ent.entnome IS NOT NULL
						  AND ent.tpcid = ".($_SESSION['par']['itrid'] == 2 ? 3 : 1)."
						  
						  ORDER BY 2 ASC";
		}
		else{
			$sql_combo = "SELECT
							ent.entid as codigo,
							ent.entnome as descricao
						  FROM entidade.entidade ent
						  INNER JOIN entidade.endereco eed ON ent.entid = eed.entid {$stWhere}
						  INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = ".FUNID_ESCOLA."
						  WHERE	ent.entstatus = 'A'
						  AND ent.entcodent IS NOT NULL
						  AND ent.entnome IS NOT NULL
						  AND ent.tpcid = ".($_SESSION['par']['itrid'] == 2 ? 3 : 1)."
						  
						  UNION ALL
		
						 SELECT
							ent.entid as codigo,
							ent.entnome as descricao
						FROM entidade.entidade ent
						INNER JOIN entidade.endereco eed ON eed.entid = ent.entid {$stWhere}
						INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND fue.funid = ".($_SESSION['par']['itrid'] == 2 ? FUNID_SECRETARIA_MUNICIPAL_EDUCACAO : FUNID_SECRETARIA_ESTADUAL_EDUCACAO)."
						WHERE ent.entstatus = 'A' 
						  
						  ORDER BY 2 ASC";
			
		}
		
		$pcuid = $_REQUEST['pcuid'];
	
		if($pcuid){
	
			$sql = "SELECT
						pcu.entid
					FROM par.pfcursista pcu
					WHERE pcu.pcuid = {$pcuid}";
	
			$escola = $db->pegaUm($sql);
		}
	
		$db->monta_combo('escola', $sql_combo, 'S', 'Selecione uma escola...', '', '', '', '', 'S', '', '', $escola);
		?>
	</div>
	<?php if($_SESSION['par']['prgid'] == 12){//ESCOLAS DE GESTORES?>
	<div id="campoAno" style="display:none;">
		<?php
		$sql = "select '2012' as codigo, '2012' as descricao
				union
				select '2013' as codigo, '2013' as descricao";
	
		$db->monta_combo('pcuano', $sql, 'S', 'Selecione o ano...', null, null, null, 200, 'S', '', '', $arPfCursista['pcuano']);
		?>
	</div>
	<?php }?>
	
	<script>
		<?php if($arPfCursista['pffid'] == 14): ?>
			jQuery('#pcufuncao').show();
			jQuery('#opcfuncao').show();
			jQuery('select[name=pffid]').hide();
		<?php endif; ?>
	
		<?php if($arPfCursista['tvpid'] == 4): ?>
			jQuery('#pcuvinculo').show();
			jQuery('#opcvinculo').show();
			jQuery('select[name=tvpid]').hide();
		<?php endif; ?>
	</script>
	<?php
	}
	?>
<?}?>
<?php 
$programa = pegarProgramaDisponivel();
$perfil = pegaArrayPerfil($_SESSION['usucpf']);

$programa = $programa ? $programa : array();
?>
<script>
<?php

	#REGRA ORIGINAL
	if($_SESSION['par']['prgid'] == PROG_PAR_ESCOLA_GESTORES || $_SESSION['par']['prgid'] == PROG_PAR_PRADIME){//12 = ESCOLAS DE GESTORES / 9 = PRADIME 
		if($saldoVagas <=0 && !$_REQUEST['pcuid']){
			echo "jQuery('#btngravar').hide();";
			echo "jQuery('#btncancelar').hide();";
		} 
	}
	
	#NOVA REGRA ADICIONA - SE O PROGRAMA ESTIVER COM A DATA DE ADESAO DISPONIVEL NÃO ENTRA E OS BOTÕES CONTINUAM VISIVEIS. 
	if(	$_SESSION['par']['prgid'] == PROG_PAR_PRADIME && !(in_array(PROGRAMA_PRADIME, $programa)	)){
		//echo "alert('oi');";
		echo "jQuery('#btngravar').hide();";
		echo "jQuery('#btncancelar').hide();";
		echo "jQuery('input, select, textarea, button').attr('disabled', true);";
	}
	
	#NOVA REGRA ADICIONA - SE O PROGRAMA ESTIVER COM A DATA DE ADESAO DISPONIVEL NÃO ENTRA E OS BOTÕES CONTINUAM VISIVEIS.
	if(	$_SESSION['par']['prgid'] == PROG_PAR_ESCOLA_GESTORES && !(in_array(PROGRAMA_ESCOLA_GESTORES, $programa)	)){
		echo "jQuery('#btngravar').hide();";
		echo "jQuery('#btncancelar').hide();";
		echo "jQuery('input, select, textarea, button').attr('disabled', true);";
	}
	
	#REGRA ORIGINAL
	if( ($_SESSION['par']['prgid'] == PROG_PAR_ALFABETIZACAO_IDADE_CERTA && $esdid != WF_TIPO_PNAIC_CADASTRAMENTO) ){// 157 = PNAIC - 483 = WF_TIPO_PNAIC_CADASTRAMENTO
		echo "jQuery('#btngravar').hide();";
		echo "jQuery('#btncancelar').hide();";
		echo "jQuery('input, select, textarea, button').attr('disabled', true);"; 
	}
	
	#NOVA REGRA ADICIONA - SE O PROGRAMA ESTIVER COM A DATA DE ADESAO DISPONIVEL NÃO ENTRA E OS BOTÕES CONTINUAM VISIVEIS.
	# -> Solicitação Wallace/Manuelita para habilitar o cadastramento sem data de termino
	# -> Solicitação pela Juliano para fechar a adesão
	if($_SESSION['par']['prgid'] == PROG_PAR_ALFABETIZACAO_IDADE_CERTA && !(in_array(PROGRAMA_ALFABETIZACAO_IDADE_CERTA, $programa)	)){
		echo "jQuery('#btngravar').hide();";
		echo "jQuery('#btncancelar').hide();";
		echo "jQuery('input, select, textarea, button').attr('disabled', true);"; 
	}

	#NOVA REGRA ADICIONA - SE O PROGRAMA ESTIVER COM A DATA DE ADESAO DISPONIVEL NÃO ENTRA E OS BOTÕES CONTINUAM VISIVEIS.
	if($_SESSION['par']['prgid'] == PROG_PAR_PROCONSELHO && !(in_array(PROGRAMA_PROCONSELHO, $programa)	)){
		echo "jQuery('#btngravar').hide();";
		echo "jQuery('#btncancelar').hide();";
		echo "jQuery('input, select, textarea, button').attr('disabled', true);"; 
	}

	#NOVA REGRA ADICIONA - SE O PROGRAMA ESTIVER COM A DATA DE ADESAO DISPONIVEL NÃO ENTRA E OS BOTÕES CONTINUAM VISIVEIS.
	if($_SESSION['par']['prgid'] == PROG_PAR_PROINFANTIL && !(in_array(PROGRAMA_PROINFANTIL, $programa)	)){
		echo "jQuery('#btngravar').hide();";
		echo "jQuery('#btncancelar').hide();";
		echo "jQuery('input, select, textarea, button').attr('disabled', true);"; 
	}	

	#NOVA REGRA ADICIONA - SE O PROGRAMA ESTIVER COM A DATA DE ADESAO DISPONIVEL NÃO ENTRA E OS BOTÕES CONTINUAM VISIVEIS.
	if($_SESSION['par']['prgid'] == PROG_PAR_PROFUNCIONARIO && !(in_array(PROGRAMA_PROFUNCIONARIO, $programa)	)){
		echo "jQuery('#btngravar').hide();";
		echo "jQuery('#btncancelar').hide();";
		echo "jQuery('input, select, textarea, button').attr('disabled', true);"; 
	}	
	
	#NOVA REGRA ADICIONA - SE O PERFIL É SUPER USUÁRIO OS BOTÕES CONTINUAM VISIVEIS.
	if( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_PROFUNC_ANALISEPF, $perfil) || in_array(PAR_PERFIL_PROFUNC_PREANALISEPF, $perfil) ){
		echo "jQuery('#btngravar').show();";
		echo "jQuery('#btncancelar').show();";
		echo "jQuery('input, select, textarea, button').attr('disabled', false);"; 
	}
	
	#NOVA REGRA ADICIONA - SE O MUNICIPIO FOR ALGUM DOS RELACIONADOS PELA MANUELITA, OS BOTÕES CONTINUAM VISIVEIS.
	//$list_muncod = array('1303700','5206602','5219738','2105609','2108454','2111201','2111706','3100906','3126950','3131158','3132800','3140530','3143708','3156502','3168101','5005004','5007505','1500305','1504422','1505494','2504405','2504850','2513604','2514453','2611101','2403509','2405306','2408953','1400282','4300901','4316303','4318614','4323754','4211256','4215059','2801009','2805208','3506607','3508900','3509403','3510153','3511201','3511607','3515301','3523701','3524303','3526506','3529658','3548203','3548302','3548807','3554706','3555356','1703842','1718451','1720853');
	//if( in_array($_SESSION['par']['muncod'], $list_muncod) ){
		//echo "jQuery('#btngravar').show();";
		//echo "jQuery('#btncancelar').show();";
		//echo "jQuery('input, select, textarea, button').attr('disabled', false);"; 
	//}


?>

	function salvarSit(){
		var numero = "<?=$_REQUEST['numero']?>";
		//alert(jQuery('#formdados').serialize());
		jQuery('#arr').val(jQuery('#formdados').serialize());
		jQuery('#numero').val(numero);
		document.formulario.submit();
	}
</script>
<div id="dialog-confirm" title="Confirmação" style="display:none;">
	<div id="dialog-content"></div>
</div>