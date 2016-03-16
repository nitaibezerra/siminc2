<?php
require_once APPRAIZ . "tarefa/classes/Solicitante.class.inc"; 
require_once APPRAIZ . "www/includes/webservice/cpf.php";
if( $_POST['destroySessionIescodEntidade'] == 't' ){
	unset( $_SESSION['iescodSessionEntidade'] );
	die();
}
if($_POST['acaoForm']){
	$boGravaCpf = false;
	extract($_POST); 
	$obSolicitante = new Solicitante();
	if($soltiposolicitante == 'E' ){ # Tipo externo
		if($soltipopessoa == 'J' ){ # Pessoa Jurídica
			if(($_GET['solid'] || intval($_GET['solid']) === 0) && $_GET['formAcao']){ # Se tiver solid é Alteração
        		alterar($_POST, $_GET['solid']);
        	} else {
          	  	if(!isset($_SESSION['arSolicitante'][0][0]['solnome'])){
          	  		$_SESSION['arSolicitante'] = array(0);
          	  	} elseif(!$_SESSION['arSolicitante'][0][0]['solnome']){
          	  		$_SESSION['arSolicitante'] = array(0);
          	  	}
          	  	if( $_SESSION['iescodSessionEntidade']){
					$iesid = $_SESSION['iescodSessionEntidade'];
          	  	}
          	  	 
          	  	$arSolicitante = $_SESSION['arSolicitante'];
				$registro = Array("solid" 			   => $solid,
								  "tarid" 			   => $tarid,
						          "soltiposolicitante" => $soltiposolicitante,
						          "soltipopessoa" 	   => $soltipopessoa,
						          "entid" 			   => $entid, 
								  "iesid" 			   => $iesid, 
						          "estuf" 			   => $estuf,
						          "muncod" 			   => $muncod,
						          "solnome" 		   => $solnome,
						          "soltelefone" 	   => $soltelefone,
						          "solemail" 		   => $solemail,
						          "solendereco" 	   => $solendereco,			
						          "solddd" 	   		   => $solddd,			
							 );
				if($arSolicitante[0] === 0){
					$arSolicitante[0] = array($registro);
				} else {
					$arSolicitante[0][] = $registro;
				}						 
				$_SESSION['arSolicitante'] = $arSolicitante;
				 
        	}
		} elseif($soltipopessoa == 'F'){ # Pessoa Física
			$solcpf = str_replace(array(".","-"),"",$solcpf);
			if(($_GET['solid'] || intval($_GET['solid']) === 0) && $_GET['formAcao']){ # Se tiver solid é Alteração
	        	alterar($_POST, $_GET['solid'], true);
			} else {
				if(!isset($_SESSION['arSolicitante'][0][0]['solnome'])){
          	  		$_SESSION['arSolicitante'] = array(0);
          	  	} elseif(!$_SESSION['arSolicitante'][0][0]['solnome']){
          	  		$_SESSION['arSolicitante'] = array(0);
          	  	}
          	  	$arSolicitante = $_SESSION['arSolicitante'];   
				$registro = Array("solid" 			   => $solid,
								  "tarid" 			   => $tarid,
						          "soltiposolicitante" => $soltiposolicitante,
						          "soltipopessoa" 	   => $soltipopessoa,
						          "estuf" 			   => $estuf,
						          "muncod" 			   => $muncod,
						          "solnome" 		   => $solnome,
						          "soltelefone" 	   => $soltelefone,
						          "solemail" 		   => $solemail,
						          "solcpf" 		       => $solcpf,
						          "solendereco" 	   => $solendereco,
						          "solddd" 	   		   => $solddd,
							 );
						
				if($arSolicitante[0] === 0){
					$arSolicitante[0] = array($registro);
				} else {
					$arSolicitante[0][] = $registro;
				}
				$_SESSION['arSolicitante'] = $arSolicitante;
			}
		}
	} else { # Tipo Interno
		if(($_GET['solid'] || intval($_GET['solid']) === 0) && $_GET['formAcao']){ # Se tiver solid é Alteração
        	alterar($_POST,$_GET['solid']);
		} else {
			if(!isset($_SESSION['arSolicitante'][0][0]['solnome'])){
            	$_SESSION['arSolicitante'] = array(0);
            } elseif(!$_SESSION['arSolicitante'][0][0]['solnome']){
            	$_SESSION['arSolicitante'] = array(0);
            }
            $arSolicitante = $_SESSION['arSolicitante'];
			$registro = Array("solid" 		   => $solid,
						  "tarid" 			   => $tarid,
				          "soltiposolicitante" => $soltiposolicitante,
				          "unaid" 	   		   => $unaid,
				          "solnome" 		   => $solnome,
				          "soltelefone" 	   => $soltelefone,
				          "solemail" 		   => $solemail,
				          "solendereco" 	   => $solendereco,
				          "solddd" 	   		   => $solddd,			
					 );
			if($arSolicitante[0] === 0){
				$arSolicitante[0] = array($registro);
			} else {
				$arSolicitante[0][] = $registro;
			}
			$_SESSION['arSolicitante'] = $arSolicitante;
		}
	}
	unset($_POST);
	unset($obSolicitante);
//	ver( $_SESSION['cadTarefa']['REQUEST_URI'],d );
// 	ver($_GET['urlPai']."&mantemIescod=t" ,d);
	echo '<script type="text/javascript">alert("Operação Realizada com Sucesso!"); ';
	echo 'window.opener.location.href="'.$_SESSION['cadTarefa']['REQUEST_URI'].'&mantemIescod=t";window.close(); </script>';  
	die;
}

function alterar($_POST, $_solid,  $boGravaCpf = false){
	extract($_POST);
	$solid = intval($_solid);
	$arSolicitanteId = $_SESSION['arSolicitante'][0][$solid];
	if($soltiposolicitante == "E"){
		if($soltipopessoa == "J"){
			$unaid = null;
			$solcpf = null;
			$arSolicitanteId['iesid'] 			   = $_SESSION['iescodSessionEntidade'];
		} else {
			$unaid = null;
			$entid = null;
		}
	} else {
		$entid = null;
		$solcpf = null;
	}
	
    $arSolicitanteId['solid'] 			   = null;
	$arSolicitanteId['tarid'] 			   = $tarid;
	
	$arSolicitanteId['soltiposolicitante'] = ($soltiposolicitante) ? $soltiposolicitante : null;
	$arSolicitanteId['soltipopessoa'] 	   = ($soltipopessoa) ? $soltipopessoa : null;
	$arSolicitanteId['unaid'] 			   = ($unaid) ? $unaid : null;
	$arSolicitanteId['estuf'] 			   = ($estuf) ? $estuf : null;
	$arSolicitanteId['muncod'] 			   = ($muncod) ? $muncod : null;
	$arSolicitanteId['solnome'] 		   = ($solnome) ? $solnome : null;
	$arSolicitanteId['soltelefone'] 	   = ($soltelefone) ? $soltelefone : null;
	$arSolicitanteId['solemail']  		   = ($solemail) ? $solemail : null;	   
	$arSolicitanteId['entid'] 			   = ($entid) ? $entid : null;
	$arSolicitanteId['solendereco'] 	   = ($solendereco) ? $solendereco : null;
	$arSolicitanteId['solddd'] 	   		   = ($solddd) ? $solddd : null;
	if($boGravaCpf){
		$arSolicitanteId['solcpf'] 		   = ($solcpf) ? str_replace(array(".","-"),"",$solcpf) : null;		
	} else {
		$arSolicitanteId['solcpf'] 		   = null;		
	}
	$_SESSION['arSolicitante'][0][$solid] = $arSolicitanteId;
}

if( $_POST['ajaxestuf'] ){	 
	header('content-type: text/html; charset=ISO-8859-1');
 
	$sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			where
			 estuf = '".$_POST['ajaxestuf']."' 
			order by
			 mundescricao asc";
	die($db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'muncod',false,null,'Município'));
}

# CARREGAMOS A PÁGINA COM A POSIÇÃO PASSADA
if(isset($_SESSION['arSolicitante']) && is_array($_SESSION['arSolicitante']) && count($_SESSION['arSolicitante'][0]) && intval($_GET['solid']) >= 0){
	$obSolicitante = new Solicitante();
	$arSolicitanteId = $_SESSION['arSolicitante'][0][$_GET['solid']];
	$arSolicitanteId = ($arSolicitanteId) ? $arSolicitanteId : array();
	foreach ($arSolicitanteId as $key => $value) {
	    $obSolicitante->$key = $value;
	}
} else {
	$obSolicitante = new Solicitante();
}

/**
 * Checked para Solicitante
 */
$ckSoltiposolicitanteE = $ckSoltiposolicitanteI = "";
if($obSolicitante->soltiposolicitante == 'E'){
	$ckSoltiposolicitanteE = "checked=\"checked\"";
} elseif($obSolicitante->soltiposolicitante == 'I') {
	$ckSoltiposolicitanteI = "checked=\"checked\"";	
} else {
	$ckSoltiposolicitanteE = "checked=\"checked\"";
}

/**
 * Checked para Tipo Pessoa
 */
$ckSoltipopessoaJ = $ckSoltipopessoaF = "";
if($obSolicitante->soltipopessoa == 'J'){
	$ckSoltipopessoaJ = "checked=\"checked\"";
} elseif($obSolicitante->soltipopessoa == 'F') {
	$ckSoltipopessoaF = "checked=\"checked\"";	
} else {
	$ckSoltipopessoaJ = "checked=\"checked\"";
}

 

?>
<html>
  <head>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
  </head>
<br>    
<form name="formulario" id="formulario" method="post">
<table  bgcolor="#f5f5f5" align="center" class="tabela" >
<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
<input type="hidden" name="tarid" id="tarid" value="<?php echo $_GET['tarid']; ?>" />
	<tr>
		<td class = "subtitulodireita" colspan="2">
			<center> 
			<h3>Cadastro de Solicitante</h3>
			</center>
		</td> 
	</tr>
	<tr>
		<td class = "subtitulodireita"> 
			Tipo de Solicitante:
		</td>
		<td>
			<input type="radio" id="soltiposolicitanteE" title="Tipo de Solicitante" <?php echo $ckSoltiposolicitanteE; ?> onclick="verificaTipoSolicitante('E');" name="soltiposolicitante" value="E" align="bottom"><label for="soltiposolicitanteE"> Externo</label>
			&nbsp;&nbsp;
			<input type="radio" id="soltiposolicitanteI" title="Tipo de Solicitante" <?php echo $ckSoltiposolicitanteI; ?> onclick="verificaTipoSolicitante('I');" name="soltiposolicitante" value="I" align="bottom"><label for="soltiposolicitanteI"> Interno</label>
			<img src="/imagens/obrig.gif">
		</td>
	</tr>
	<tr id="tr_tipo_pessoa">
		<td class = "subtitulodireita"> 
			 Tipo de Pessoa:
		</td>
		<td>
			<input type="radio" id="soltipopessoaJ" title="Tipo de Pessoa" <?php echo $ckSoltipopessoaJ; ?> onclick="verificaTipoPessoa('J');" name="soltipopessoa" value="J" align="bottom"><label for="soltipopessoaJ"> Jurídica</label>
			&nbsp;&nbsp;
			<input type="radio" id="soltipopessoaF" title="Tipo de Pessoa" <?php echo $ckSoltipopessoaF; ?>onclick="verificaTipoPessoa('F');"  name="soltipopessoa" value="F" align="bottom"><label for="soltipopessoaF"> Física</label>
			<img src="/imagens/obrig.gif">
		</td>
	</tr>
	<tr id="unidade">
		<td class = "subtitulodireita"> 
			 Unidade:
		</td>
		<td><?
			$unaid = $obSolicitante->unaid;
			$sql = "SELECT unaid as codigo, unasigla||' - '|| unadescricao as descricao FROM tarefa.unidade order by descricao";
			$db->monta_combo( "unaid", $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'unaid',false,null,'Unidade');
			?>
		</td>
	</tr>
	<tr id="entidade">
		<td class = "subtitulodireita"> 
			 Entidade:
		</td>
		<td><?
			$entid = $obSolicitante->entid;
			$sql = "select e.entid as codigo, e.entnome as descricao from entidade.entidade e 
					 inner join tarefa.entidadesolicitante es on e.entid = es.entid 
					 union all select '0' as codigo, 'Instituição de Ensino Superior' as descricao 
					 order by descricao 
					 ";
			$db->monta_combo( "entid", $sql, 'S', 'Selecione...', 'verificaIES(this.value);', '', '', '', 'S', 'entid',false,null,'Entidade');
			?>
			<br>
			<br>
			<div id="div_instituicao" >
			<?php
			if( $obSolicitante->iesid ){
				$sql = "select iesid, iescodigo, iessigla, iesnome, iesuf FROM ies.ies where iesid = ".$obSolicitante->iesid;
				$rs  = $db->carregar( $sql );
				if( $rs ){ 
					$_SESSION['iescodSessionEntidade'] = $obSolicitante->iesid;
					$resp = "<a href=\"javascript: abrirPopupInstituicao('radio');\">".$rs[0]['iescodigo']." - ".$rs[0]['iessigla']." - ".$rs[0]['iesnome']."</a>";
					echo( $resp );
				}
			}
			
			?>
			</div>
		</td>
	</tr>
	<tr id="entidade_cpf" style="display: none;">
		<td class = "subtitulodireita"> 
			 CPF:
		</td>
		<td> 
		  <input id="solcpf" title="CPF" readonly="readonly" class="normal" type="text" name="solcpf" style="width: 21ex;" size="19" value="<?php echo $obSolicitante->solcpf; ?>" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" onblur="MouseBlur(this); validarsolcpf( this );"/>
		  <span style="cursor:pointer" onclick="abrirPopupEntidade();"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Adicionar Solicitante por CPF</span>
		</td>
	</tr>
	<tr id="tr_estado">
		<td class = "subtitulodireita"> 
			 Estado:
		</td>
		<td>
		<?
			 $estuf = $obSolicitante->estuf;
			 $sql = "select
					 e.estuf as codigo, e.estdescricao as descricao 
					from
					 territorios.estado e 
					order by
					 e.estdescricao asc";
			 $db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraTipo', '', '', '', 'S', 'estuf',false,null,'Estado');
			 ?>						
		</td>
	</tr>
	<tr id="tr_municipio" >
	<td class = "subtitulodireita">
			Município:
			<br/>
		</td>
		<td  id="municipio">
			<?
			if ($obSolicitante->estuf) {
				$sql = "select
						 muncod as codigo, 
						 mundescricao as descricao 
						from
						 territorios.municipio
						where
						 estuf = '".$obSolicitante->estuf."' 
						order by
						 mundescricao asc";
				$muncod = $obSolicitante->muncod;
				$db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '','','S', 'muncod',false,null,'Município');
			} else {
				$db->monta_combo( "muncod", array(), 'S', 'Selecione o Estado', '', '', '', '', 'S', 'muncod',false,null,'Município');				
			}
			?>	
		</td>	
	</tr>
	<tr id="tr_nome">
		<td class = "subtitulodireita"> 
			 Nome do Solicitante:
		</td>
		<td>
			<input id="solnome" title="Nome do Solicitante" class="normal" type="text" name="solnome" size="60" maxlength="50" value="<?php echo $obSolicitante->solnome; ?>"/>&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
		</td>
	</tr>
	<tr>
		<td class = "subtitulodireita"> 
			 (DDD) Telefone de Contato do Solicitante:
		</td>
		<td>
			<? 
				$soltelefone = $obSolicitante->soltelefone;
				//echo campo_texto('soltelefone', 'N', $permissaogravar, 'Telefone de Contato do Solicitante', 60, 30, '####-####', '', 'left', '', 0, ''); 
			?>
			<input id="solddd" title="DDD de Contato do Solicitante" class="normal" onkeyup="this.value=mascaraglobal('####',this.value);" type="text" name="solddd" size="2" maxlength="2" value="<?php echo $obSolicitante->solddd; ?>"/>
			<input id="soltelefone" title="Telefone de Contato do Solicitante" class="normal" onkeyup="this.value=mascaraglobal('####-####',this.value);" type="text" name="soltelefone" size="55" maxlength="30" value="<?php echo $obSolicitante->soltelefone; ?>"/>
		</td>
	</tr>
	<tr>
		<td class = "subtitulodireita"> 
			 Email de Contato do Solicitante:
		</td>
		<td>
			 <?
			 	$solemail = $obSolicitante->solemail; 
			 	//echo campo_texto('solemail', 'N', $permissaogravar, 'Email de Contato do Solicitante', 60, 100, '', '', 'left', '', 0, ''); 
			 ?>
			 <input id="solemail" title="Email de Contato do Solicitante" class="normal" type="text" name="solemail" size="60" maxlength="100" value="<?php echo $obSolicitante->solemail; ?>"/>
		</td>
	</tr>
	<tr>
		<td class = "subtitulodireita"> 
			 Endereço do Solicitante:
		</td>
		<td>
			<input id="solendereco" title="Nome do Solicitante" class="normal" type="text" name="solendereco" size="60" maxlength="100" value="<?php echo $obSolicitante->solendereco; ?>"/>
		</td>
	</tr>
 	 
	<tr>
		<td class="SubTituloDireita" colspan="2" style="text-align:center">
		 	 <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="enviaForm();">  
			 <input type="button" name="btnFechar" value="Cancelar" id="btnFechar" onclick="fecharPopup();">  
		</td>
	</tr>
	 
		    
</table>
<script type="text/javascript">
<?if( $obSolicitante->iesid ) {?>
	document.getElementById("entid").value=0;
<?} ?> 
function fecharPopup(){
	return window.close();
}
function verificaIES(tipo){ 
	if( !tipo ){
		return false;
	}
	if( tipo == 0 ){
		abrirPopupInstituicao('radio');
	}else if(tipo != 0){
		var req = new Ajax.Request('tarefa.php?modulo=principal/popupSolicitante&acao=A', {
							        method:     'post',
							        parameters: '&destroySessionIescodEntidade=t',
							        onComplete: function (res)
							        {		  
										document.getElementById('div_instituicao').innerHTML = " ";
							        }
							  });
	}	
}
function verificaTipoSolicitante( tipo ){
	if( tipo == 'I' ){
		document.getElementById('tr_tipo_pessoa').style.display = 'none';
		document.getElementById('tr_estado').style.display = 'none';
		document.getElementById('tr_municipio').style.display = 'none';
		document.getElementById('entidade_cpf').style.display = 'none';
		document.getElementById('entidade').style.display = 'none';
		document.getElementById('unidade').style.display = '';
	}else if( tipo == 'E' ){ 
		document.getElementById('tr_tipo_pessoa').style.display = '';
		document.getElementById('tr_estado').style.display = '';
		document.getElementById('tr_municipio').style.display = '';
		document.getElementById('entidade').style.display = '';
		document.getElementById('unidade').style.display = 'none';
	} else {
		document.getElementById('tr_tipo_pessoa').style.display = '';
		document.getElementById('tr_estado').style.display = '';
		document.getElementById('tr_municipio').style.display = '';
		document.getElementById('entidade').style.display = '';
		document.getElementById('unidade').style.display = 'none';	
	}
}
function verificaTipoPessoa( tipo ){
	if( $('soltipopessoaJ').checked == true ){
		document.getElementById('entidade_cpf').style.display = 'none';
		document.getElementById('unidade').style.display = 'none';
		document.getElementById('entidade').style.display = '';	
	} else if( tipo == 'F' ){
		document.getElementById('entidade_cpf').style.display = '';
		document.getElementById('unidade').style.display = 'none';
		document.getElementById('entidade').style.display = 'none';
	} else if( tipo == 'J' ){
		document.getElementById('entidade_cpf').style.display = 'none';
		document.getElementById('unidade').style.display = 'none';
		document.getElementById('entidade').style.display = '';
	} else {
		document.getElementById('entidade_cpf').style.display = 'none';
		document.getElementById('unidade').style.display = 'none';
		document.getElementById('entidade').style.display = '';
	}
}

function filtraTipo(estuf) {
	if( !estuf ){
		return false;
	}
	td 	   = document.getElementById('municipio');
	select = document.getElementsByName('muncod')[0];
	
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('tarefa.php?modulo=principal/popupSolicitante&acao=A', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,
							        onComplete: function (res)
							        {			  
										td.innerHTML = res.responseText;
										td.style.visibility = 'visible';
							        }
							  });
    
}
function abrirPopupInstituicao(tipo){
	new Ajax.Request('ajax.php',
	{  
		method: 'post',   
		parameters: '',   
		onComplete: function(r)
		{ 
			window.open('tarefa.php?modulo=principal/popupInstituicoes&acao=A&type='+tipo,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=700,height=500');
	 
		}
	});
}
 
function enviaForm(){
 	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "soltiposolicitante";
	campos[1] 			= "soltipopessoa";
	campos[2] 			= "solnome";
	
	if(document.getElementById('soltiposolicitanteE').checked == true){
		if(document.getElementById('soltipopessoaJ').checked == true){
			campos[3] 			= "entid";
		}
		if( document.getElementById('entid').value != 0 ){
			campos[4] 			= "estuf";
			campos[5] 			= "muncod";
		}
	} else {
		campos[3] 			= "unaid";	
	}
	
	tiposDeCampos[0] 	= "radio";
	tiposDeCampos[1] 	= "radio";
	tiposDeCampos[2] 	= "texto";
	
	if(document.getElementById('soltiposolicitanteE').checked == true){
		if(document.getElementById('soltipopessoaJ').checked == true){
			tiposDeCampos[3] 	= "select";
		}
		tiposDeCampos[4] 	= "select";
		tiposDeCampos[5] 	= "select";
	} else {
		tiposDeCampos[3] 	= "select";	
	}
	<?php $_SESSION['mantemIescod'] = 't'; ?>
	validaForm(nomeform, campos, tiposDeCampos, submeterForm );
	
}

function validarsolcpf( obj ){
	if( obj.value ){
		if( !validar_cpf( obj.value  ) ){
			alert( "CPF inválido!\nFavor informar um cpf válido!" );
			obj.value = "";	
		}
	}
}

function abrirPopupEntidade(){
	window.open('tarefa.php?modulo=principal/popupEntidade&acao=A','','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=700,height=800');
}

var tiposolicitante = '<?php echo $obSolicitante->soltiposolicitante; ?>';
verificaTipoSolicitante(tiposolicitante);

var tipopessoa = '<?php echo $obSolicitante->soltipopessoa; ?>';
verificaTipoPessoa(tipopessoa);

document.getElementById('solcpf').value = mascaraglobal('###.###.###-##',document.getElementById('solcpf').value);
</script>