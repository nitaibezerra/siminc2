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
				
				$sql = "UPDATE par.pfentidade
						SET pscid = ".$ar[1]." 
						WHERE pfeid = ".substr($ar[0],$start,($end-$start));
				$db->executar($sql);
				$flag = true;
			}
		}
	}
	
	if($flag) {
		$db->commit();
		unset($_POST['arr']);
		//echo "<script>alert('Operação efetuada com sucesso.');</script>";
		
		die("<script>
				alert('Operação efetuada com sucesso.');
				location.href = 'par.php?modulo=principal/programas/feirao_programas/professortutor&acao=A&numero=".$_REQUEST['numero']."';
			</script>");
			
		
	}
	
}



if(!$_SESSION['par']['adpid']){
	echo '<script type="text/javascript"> 
    		alert("Sessão Expirou.\nFavor selecione o programa novamente!");
    		window.location.href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa";
    	  </script>';
    die;
}




require_once APPRAIZ . "includes/classes/entidades.class.inc";

ini_set( "memory_limit", "1024M" );

//pega estado atual
$docid = pgCriarDocumento( $_SESSION['par']['adpid'] );
$esdid = prePegarEstadoAtual( $docid );




$obEntidade 	= new Entidades();
$obPfEntidade 	= new PfEntidade();

$arPfEntidade = $obPfEntidade->recuperarPorAdpid($_SESSION['par']['adpid'], FUNID_PROF_TUTOR_PRO_FUNCIONARIO, $_REQUEST['pfeid']);

if ($_REQUEST['opt'] == 'excluir') {


	if($_REQUEST['entid']){

	    //$obPfEntidade->removerFuncaoEntidade($_REQUEST['entid'], FUNID_PROF_TUTOR_PRO_FUNCIONARIO);
	    //$obPfEntidade->commit();
		$obPfEntidade->excluir($_REQUEST['pfeid']);
		$obPfEntidade->commit();

		echo "<script>
				alert('Dados excluidos com sucesso.');
				document.location.href = '?modulo=principal/programas/feirao_programas/professortutor&acao=A';
			  </script>";

	}
	exit();
}

if ($_REQUEST['opt'] == 'salvarRegistro') {

	$arEscolas = $_REQUEST['escola'];

	if($_REQUEST['entid']){
		$obPfEntidade->limpaEntidadeAssocPorEntid($_REQUEST['entid']);
	}

	$obEntidade->carregarEntidade($_REQUEST);
	if(count($arEscolas) && is_array($arEscolas)){

		foreach($arEscolas as $escola){

			$_REQUEST['funcoes']['funid'] = FUNID_PROF_TUTOR_PRO_FUNCIONARIO;
			$_REQUEST['funcoes']['entidassociado'] = $escola;

			$obEntidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
		}

	}else{

		$_REQUEST['funcoes']['funid'] = FUNID_PROF_TUTOR_PRO_FUNCIONARIO;
		$_REQUEST['funcoes']['entidassociado'] = $_REQUEST['escola'];

		$obEntidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	}
	$obEntidade->salvar();

	$boEntid = $obPfEntidade->verificaEntidadeExiste($obEntidade->getEntId(), $_SESSION['par']['adpid'], FUNID_PROF_TUTOR_PRO_FUNCIONARIO);

	if(!$boEntid || $_REQUEST['pfeid']){

		$obPfEntidade->pfeid = $_REQUEST['pfeid'] ? $_REQUEST['pfeid'] : null;
		$obPfEntidade->entid = $obEntidade->getEntId();
		$obPfEntidade->adpid = $_SESSION['par']['adpid'];
		$obPfEntidade->tvpid = $_REQUEST['tvpid'];
		$obPfEntidade->pffid = $_REQUEST['pffid'];
		$obPfEntidade->tfoid = $_REQUEST['tfoid'];
		$obPfEntidade->pfefuncao = $_REQUEST['pffid'] == 14 ? $_REQUEST['pfefuncao'] : "";
		$obPfEntidade->pfevinculo = $_REQUEST['tvpid'] == 4 ? $_REQUEST['pfevinculo'] : "";
		$obPfEntidade->salvar();
		$obPfEntidade->commit();

		echo "<script>
				alert('Dados gravados com sucesso.');
				document.location.href = '?modulo=principal/programas/feirao_programas/professortutor&acao=A';
			  </script>";

	}else{

		echo "<script>
				alert('Professor já cadastrado.');
				document.location.href = '?modulo=principal/programas/feirao_programas/professortutor&acao=A';
			  </script>";

	}
	exit();
}
include_once "termoprofuncionario.inc";

$aderiu = testaAdesaoProFuncionario();
if( $aderiu == 'S' ){
include_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br>';
echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/professortutor&acao=A');
monta_titulo( $titulo_modulo, recuperaMunicipioEstado() );


?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--

jQuery.noConflict();

jQuery(document).ready(function(){

	var cpFormacao 	 = jQuery('#campoFormacao').html();
	var cpVinculo 	 = jQuery('#campoVinculo').html();
	var cpFuncao 	 = jQuery('#campoFuncao').html();
	var campoEscolas = jQuery('#campoEscolas').html();

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
			jQuery('#pfefuncao').show();
			jQuery('#opcfuncao').show();
			this.hide();
		}
	});

	jQuery('select[name=tvpid]').live('change', function(){
		if(this.value == 4){
			jQuery('#pfevinculo').show();
			jQuery('#opcvinculo').show();
			this.hide();
		}
	});

	jQuery('.mostraOpcoes').live('click', function(){

		if(this.id == 'opcfuncao'){

			this.hide();
			jQuery('#pfefuncao').hide();
			jQuery('select[name=pffid]').show();
			jQuery('select[name=pffid]').val('');

		} else if(this.id == 'opcvinculo'){

			this.hide();
			jQuery('#pfevinculo').hide();
			jQuery('select[name=tvpid]').show();
			jQuery('select[name=tvpid]').val('');
		}
	});

	jQuery('#tr_entnumdddcomercial').find('td:first').html('Telefone :');

	jQuery('#tr_entobs').after('<tr id="tr_formacao"><td class="SubTituloDireita" width="30%">Formação :</td><td>'+cpFormacao+'</td></tr>');

	jQuery('#tr_formacao').after('<tr id="tr_vinculo"><td class="SubTituloDireita">Vinculo :</td><td>'+cpVinculo+'</td></tr>');

	jQuery('#tr_vinculo').after('<tr id="tr_funcao"><td class="SubTituloDireita">Função :</td><td>'+cpFuncao+'</td></tr>');

	jQuery('#tr_funcao').after('<tr id="tr_escolas"><td class="SubTituloDireita">Escolas :</td><td>'+campoEscolas+'</td></tr>');

	jQuery('#tr_acoes').after('<tr id="tr_hidden"><td colspan="2"><input type="hidden" name="pfeid" value="<?php echo $arPfEntidade['pfeid'] ?>" /></td></tr>');

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

	jQuery('#escola_campo_excludente').parent('td').hide();

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

		if(jQuery('#pfevinculo').css('display') != 'none'){
			if(jQuery('#pfevinculo').val()  == ''){
				alert('O campo Vínculo é obrigatório.');
				jQuery('#pfevinculo').focus();
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

		if(jQuery('#pfefuncao').css('display') != 'none'){
			if(jQuery('#pfefuncao').val()  == ''){
				alert('O campo Função é obrigatório.');
				jQuery('#pfefuncao').focus();
				return false;
			}
		}

//		selectAllOptions( document.getElementById( 'escola' ) );
//
//		var escola = jQuery( '#escola option:selected' );
//
//		if(escola[0].value == ''){
//			alert('O campo Escola é obrigatório.');
//			jQuery('#escola').focus();
//			return false;
//		}
//
//		if(escola.length > 1){
//			alert("O campo escola não pode ter mais de uma escola.");
//			jQuery('#escola').focus();
//			return false;
//		}

		if(jQuery('select[name=escola]').val() == ''){
			alert('O campo Escola é obrigatório.');
			jQuery('select[name=escola]').focus();
			return false;
		}
	});

	var arDados;
	jQuery('.alterar').click(function(){
	    arDados = this.id.split('_');
		document.location.href = '?modulo=principal/programas/feirao_programas/professortutor&acao=A&pfeid='+arDados[0];
	});

	jQuery('.excluir').click(function(){
	    arDados = this.id.split('_');
		document.location.href = '?modulo=principal/programas/feirao_programas/professortutor&acao=A&opt=excluir&pfeid='+arDados[0]+'&entid='+arDados[1];
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

});
</script>

<?php

$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);


$permissaoPreAnalise = 0;
if( in_array(PAR_PERFIL_PROFUNC_PREANALISEPF_TUTOR,$arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ){
	$permissaoPreAnalise = 1;
}

$permissaoAnalise = 0;
if( in_array(PAR_PERFIL_PROFUNC_ANALISEPF_TUTOR,$arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ){
	$permissaoAnalise = 1;
}

$entid = $arPfEntidade['entid'];

if($entid){
	$obEntidade->carregarPorEntid($entid);
}

$funid = FUNID_PROF_TUTOR_PRO_FUNCIONARIO;

$funentassoc = $funentassoc ? $funentassoc : null;

echo "<table border=0 cellpading=0 cellspacing=0 width='100%'>
		<tr><td width='90%'>";

	if($esdid == WF_TIPO_PROFUNC_CADASTRAMENTO || !$esdid){
		
		echo $obEntidade->formEntidade("?modulo=principal/programas/feirao_programas/professortutor&acao=A&opt=salvarRegistro",
								 array("funid" => $funid, "entidassociado" => $funentassoc),
								 array( "enderecos"=>array(1) ) );
	}
						 
echo "</td>";
echo "<td width='10%'>";	

$dados_wf = array('adpid' => $_SESSION['par']['adpid']);
wf_desenhaBarraNavegacao(pgCriarDocumento( $_SESSION['par']['adpid'] ), $dados_wf);

echo "</td></tr></table>";


echo "<form name='formulario' method='post' action=>";
echo "<input type=hidden name=arr id=arr>";
echo "<input type=hidden name=numero id=numero>";

if($esdid == WF_TIPO_PROFUNC_CADASTRAMENTO || !$esdid){

	$sql = "SELECT DISTINCT
				'<img src=\"../imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\" class=\"excluir\" id=\"' || pfe.pfeid || '_' || ent.entid || '\" style=\"cursor:pointer\" />
				 <img src=\"../imagens/alterar.gif\" alt=\"Alterar\" title=\"Alterar\" class=\"alterar\" id=\"' || pfe.pfeid || '\" style=\"cursor:pointer\"/>' as acao,
				ent.entnumcpfcnpj,
				ent.entnome,
				ent.entemail,
				'(' || ent.entnumdddcomercial || ') ' || ent.entnumcomercial as telefone,
				tfo.tfodsc,
				case when pfe.tvpid = 4 then pfe.pfevinculo
				  else pfv.tvpdsc
				end as tvpdsc,
				case when pfe.pffid = 14 then pfe.pfefuncao
				 else pff.pffdescricao
				end as pffdescricao,
				ent2.entnome as escola
			FROM par.pfentidade pfe
				INNER JOIN public.tipovinculoprofissional	pfv ON pfv.tvpid = pfe.tvpid
				INNER JOIN par.pffuncao						pff ON pff.pffid = pfe.pffid
				INNER JOIN public.tipoformacao				tfo ON tfo.tfoid = pfe.tfoid
				INNER JOIN entidade.entidade 				ent ON ent.entid = pfe.entid
				LEFT JOIN entidade.endereco 				eed ON eed.entid = ent.entid
				INNER JOIN entidade.funcaoentidade 			efu ON efu.entid = ent.entid AND efu.funid = ".FUNID_PROF_TUTOR_PRO_FUNCIONARIO."
				INNER JOIN entidade.funcao 					fun ON fun.funid = efu.funid
				LEFT JOIN entidade.funentassoc 				fea ON fea.fueid = efu.fueid
				LEFT JOIN entidade.entidade         		ent2 ON ent2.entid = fea.entid
				LEFT JOIN entidade.endereco         		ende2 ON ende2.entid = ent2.entid
				LEFT JOIN entidade.funcaoentidade 			efu2 ON efu2.entid = ent2.entid
				LEFT JOIN entidade.funcao 					fun2 ON fun2.funid = efu2.funid
				INNER JOIN par.pfadesaoprograma 				ade ON ade.adpid = pfe.adpid
			WHERE pfe.adpid = {$_SESSION['par']['adpid']} and ade.inuid = {$_SESSION['par']['inuid']}";
	//ver($sql,d);
	$cabecalho = array( "Ação", "CPF", "Nome", "Email", "Telefone", "Formação", "Vínculo", "Função", "Escola");
}else{
	$sql = "SELECT DISTINCT
				ent.entnumcpfcnpj,
				ent.entnome,
				ent.entemail,
				'(' || ent.entnumdddcomercial || ') ' || ent.entnumcomercial as telefone,
				tfo.tfodsc,
				case when pfe.tvpid = 4 then pfe.pfevinculo
				  else pfv.tvpdsc
				end as tvpdsc,
				case when pfe.pffid = 14 then pfe.pfefuncao
				 else pff.pffdescricao
				end as pffdescricao,
				ent2.entnome as escola,
				(CASE WHEN pscdescricao <> '' THEN
	    					pscdescricao
	    				  ELSE
	    				  	'Não Cadastrado'
	    			END) as sitatual,
	    			(CASE WHEN ".$esdid."=".WF_TIPO_PROFUNC_PREANALISE." OR ".$esdid."=".WF_TIPO_PROFUNC_ANALISE." OR ".$esdid."=".WF_TIPO_PROFUNC_MATRICULA." THEN
			    			'<select name=pfeidArray['||pfe.pfeid ||']>' ||
			    			(CASE WHEN ".$esdid."=".WF_TIPO_PROFUNC_PREANALISE." AND 1=$permissaoPreAnalise THEN
			    					(CASE WHEN pfe.pscid = 1 THEN
			    				  			'<option value=1 selected>Pré-qualificado</option><option value=2>Não qualificado</option>'
			    				  		  ELSE
			    				  			'<option value=1>Pré-qualificado</option><option value=2 selected>Não qualificado</option>'
			    				  	END)
			    				  WHEN ".$esdid."=".WF_TIPO_PROFUNC_ANALISE." AND 1=$permissaoAnalise THEN
			    				  	(CASE WHEN pfe.pscid  = 3 THEN
			    				  			'<option value=3 selected>Qualificado para matrícula</option><option value=4>Não qualificado para matrícula</option>'
			    				  		  WHEN pfe.pscid in (1,4)  THEN
			    				  			'<option value=3>Qualificado para matrícula</option><option value=4 selected>Não qualificado para matrícula</option>'	
			    				  		  ELSE
			    				  		  	'<option value=></option>'
			    				  	END)
			    				  WHEN ".$esdid."=".WF_TIPO_PROFUNC_MATRICULA." AND 1=$permissaoAnalise THEN
			    				  	(CASE WHEN pfe.pscid = 5 THEN
			    				  			'<option value=5 selected>Matriculado</option><option value=6>Não matrículado</option>'		
			    				  		  WHEN pfe.pscid in (3,6) THEN
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
			FROM par.pfentidade pfe
				INNER JOIN public.tipovinculoprofissional	pfv ON pfv.tvpid = pfe.tvpid
				INNER JOIN par.pffuncao						pff ON pff.pffid = pfe.pffid
				INNER JOIN public.tipoformacao				tfo ON tfo.tfoid = pfe.tfoid
				INNER JOIN entidade.entidade 				ent ON ent.entid = pfe.entid
				LEFT JOIN entidade.endereco 				eed ON eed.entid = ent.entid
				INNER JOIN entidade.funcaoentidade 			efu ON efu.entid = ent.entid AND efu.funid = ".FUNID_PROF_TUTOR_PRO_FUNCIONARIO."
				INNER JOIN entidade.funcao 					fun ON fun.funid = efu.funid
				LEFT JOIN entidade.funentassoc 				fea ON fea.fueid = efu.fueid
				LEFT JOIN entidade.entidade         		ent2 ON ent2.entid = fea.entid
				LEFT JOIN entidade.endereco         		ende2 ON ende2.entid = ent2.entid
				LEFT JOIN entidade.funcaoentidade 			efu2 ON efu2.entid = ent2.entid
				LEFT JOIN entidade.funcao 					fun2 ON fun2.funid = efu2.funid
				INNER JOIN par.pfadesaoprograma 				ade ON ade.adpid = pfe.adpid
				LEFT JOIN par.pfsituacaocursista			pfc ON pfc.pscid = pfe.pscid
			WHERE pfe.adpid = {$_SESSION['par']['adpid']} and ade.inuid = {$_SESSION['par']['inuid']}";
	//ver($sql,d);
	$cabecalho = array("CPF", "Nome", "Email", "Telefone", "Formação", "Vínculo", "Função", "Escola", "Situação Atual", "Nova Situação");
	
}
//$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
?>
</form>
<?php $db->monta_lista($sql,$cabecalho,50,5,'N','95%',"","formdados"); ?>
<center>
	
	<?if( ($esdid == WF_TIPO_PROFUNC_PREANALISE || $esdid == WF_TIPO_PROFUNC_ANALISE || $esdid == WF_TIPO_PROFUNC_MATRICULA ) &&
			($permissaoPreAnalise == 1 || $permissaoAnalise == 1) &&
			$_REQUEST['pfcid'] != 5 ) {?>
		
		<br>
		<input type="button" value="Salvar" onclick="salvarSit();">
	
	<?}?>	
	
</center>


<div id="campoFormacao" style="display:none;">
	<?php
	$sql = "select
		tfoid as codigo,
		tfodsc as descricao
	from public.tipoformacao
	where tfostatus = 'A'
	order by tfodsc";

	$db->monta_combo('tfoid', $sql, 'S', 'Selecione uma formação...', null, null, null, 200, 'S', '', '', $arPfEntidade['tfoid']);
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

	$db->monta_combo('tvpid', $sql, 'S', 'Selecione um vínculo...', null, null, null, 200, 'N', '', '', $arPfEntidade['tvpid']);
	?>
	<input type="text" class="normal obrigatorio" id="pfevinculo" name="pfevinculo" style="width: 50ex; display: none;" value="<?php echo $arPfEntidade['pfevinculo']?>" />
	<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
	<a href="javascript:void(0)" class="mostraOpcoes" id="opcvinculo" style="display:none;">Mostrar opções</a>
</div>
<div id="campoFuncao" style="display:none;">
	<?php
	$sql = "select
				pffid as codigo,
				pffdescricao as descricao
			from par.pffuncao
			where pffstatus = 'A'
			order by pffdescricao";

	$db->monta_combo('pffid', $sql, 'S', 'Selecione uma funcao...', null, null, null, 200, 'N', '', '', $arPfEntidade['pffid']);
	?>
	<input type="text" class="normal obrigatorio" id="pfefuncao" name="pfefuncao" style="width: 50ex; display: none;" value="<?php echo $arPfEntidade['pfefuncao']?>" />
	<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
	<a href="javascript:void(0)" class="mostraOpcoes" style="display:none;" id="opcfuncao">Mostrar opções</a>
</div>
<div id="campoEscolas" style="display:none;">
	<?php

	if($_SESSION['par']['itrid'] == 2){
		//$stWhere = "AND eed.muncod = '{$_SESSION['par']['muncod']}' AND eed.estuf = '{$_SESSION['par']['estuf']}'";
		$stWhere = "AND eed.muncod = '{$_SESSION['par']['muncod']}'";
	}else{
		$stWhere = "AND eed.estuf = '{$_SESSION['par']['estuf']}'";
	}
	
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

				ORDER BY 2 ASC
				  ";	
	
	//dbg($sql_combo,1);

	/*
	$sql_combo = "SELECT codigo, descricao FROM ( SELECT
					ent.entid as codigo,
					ent.entcodent || ' - ' || ent.entnome as descricao
				  FROM entidade.entidade ent
				  INNER JOIN entidade.endereco eed ON ent.entid = eed.entid {$stWhere}
				  INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = ".FUNID_ESCOLA."
				  WHERE	ent.entstatus = 'A'
				  AND ent.entcodent IS NOT NULL
				  AND ent.entnome IS NOT NULL
				  ORDER BY ent.entnome ASC ) AS foo

				  UNION ALL

				  SELECT codigo, descricao FROM ( SELECT
					ent.entid as codigo,
					ent.entnome as descricao
				FROM entidade.entidade ent
				INNER JOIN entidade.endereco eed ON eed.entid = ent.entid {$stWhere}
				INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND fue.funid = ".($_SESSION['par']['itrid'] == 2 ? FUNID_SECRETARIA_MUNICIPAL_EDUCACAO : FUNID_SECRETARIA_ESTADUAL_EDUCACAO)."
				WHERE ent.entstatus = 'A' ) as foo2";
	*/	
	//dbg($sql_combo,1);

//	if ( $arPfEntidade['entid'] )
//	{
//		$sql_carregados = "SELECT DISTINCT
//								ent2.entid as codigo,
//								ent2.entcodent || ' - ' || ent2.entnome as descricao
//							FROM entidade.entidade ent
//								INNER JOIN entidade.endereco 		eed ON eed.entid = ent.entid
//								INNER JOIN entidade.funcaoentidade 	efu ON efu.entid = ent.entid AND efu.funid = ".FUNID_PROF_TUTOR_PRO_FUNCIONARIO."
//								INNER JOIN entidade.funcao 			fun ON fun.funid = efu.funid
//								LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = efu.fueid
//								LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid
//								LEFT JOIN entidade.endereco         ende2 ON ende2.entid = ent2.entid
//								LEFT JOIN entidade.funcaoentidade 	efu2 ON efu2.entid = ent2.entid
//								LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = efu2.funid
//							WHERE ent.entid = {$arPfEntidade['entid']}";
//
//		$escola = $db->carregar( $sql_carregados );
//	}
//	combo_popup( 'escola', $sql_combo, 'Selecione a(s) Escola(s)', '400x400', 0, array(), '', 'S', true, true, 7, 400, null, null, null, null, null, true, false, null, true );

	$arPfEntidade['entid'] = $arPfEntidade['entid'] ? $arPfEntidade['entid'] : $obEntidade->getEntId();

	if($arPfEntidade['entid']){

		$sql = "SELECT DISTINCT
					ent2.entid as codigo
				FROM entidade.entidade ent
					LEFT JOIN entidade.endereco 		eed ON eed.entid = ent.entid
					INNER JOIN entidade.funcaoentidade 	efu ON efu.entid = ent.entid AND efu.funid = ".FUNID_PROF_TUTOR_PRO_FUNCIONARIO."
					INNER JOIN entidade.funcao 			fun ON fun.funid = efu.funid
					LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = efu.fueid
					LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid
					LEFT JOIN entidade.endereco         ende2 ON ende2.entid = ent2.entid
					LEFT JOIN entidade.funcaoentidade 	efu2 ON efu2.entid = ent2.entid
					LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = efu2.funid
				WHERE ent.entid = {$arPfEntidade['entid']}";
		$escola = $db->pegaUm($sql);
	}
	$db->monta_combo('escola', $sql_combo, 'S', 'Selecione uma escola...', '', '', '', '', 'S', '', '', $escola);
	?>
</div>
<script>
	<?php if($arPfEntidade['pffid'] == 14): ?>
		jQuery('#pfefuncao').show();
		jQuery('#opcfuncao').show();
		jQuery('select[name=pffid]').hide();
	<?php endif; ?>

	<?php if($arPfEntidade['tvpid'] == 4): ?>
		jQuery('#pfevinculo').show();
		jQuery('#opcvinculo').show();
		jQuery('select[name=tvpid]').hide();
	<?php endif;

	$programa = pegarProgramaDisponivel();
	$programa = $programa ? $programa : array();
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	#NOVA REGRA ADICIONA - SE O PROGRAMA ESTIVER COM A DATA DE ADESAO DISPONIVEL NÃO ENTRA E OS BOTÕES CONTINUAM VISIVEIS.
	if($_SESSION['par']['prgid'] == PROG_PAR_PROFUNCIONARIO && !(in_array(PROGRAMA_PROFUNCIONARIO, $programa)	)){
		echo "jQuery('#btngravar').hide();";
		echo "jQuery('#btncancelar').hide();";
		echo "jQuery('input, select, textarea, button').attr('disabled', true);"; 
	}else{
		echo "jQuery('#btngravar').show();";
		echo "jQuery('#btncancelar').show();";
		echo "jQuery('input, select, textarea, button').attr('disabled', false);";
	}
	
	#NOVA REGRA ADICIONA - SE O PERFIL É SUPER USUÁRIO OS BOTÕES CONTINUAM VISIVEIS.
	if(in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)){
		echo "jQuery('#btngravar').show();";
		echo "jQuery('#btncancelar').show();";
		echo "jQuery('input, select, textarea, button').attr('disabled', false);"; 
	}
	
	?>
</script>
<?php
}
?>

<script>
	function salvarSit(){
		var numero = "<?=$_REQUEST['numero']?>";
		//alert(jQuery('#formdados').serialize());
		jQuery('#arr').val(jQuery('#formdados').serialize());
		jQuery('#numero').val(numero);
		document.formulario.submit();
	}
</script>