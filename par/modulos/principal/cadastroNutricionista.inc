<?php
// Requisicao Ajax para validação
if($_REQUEST['requisicao']== 'validacaoNutricionista')
{
	$sqlUp = 
	"
		UPDATE 
			par.dadosnutricionista
		SET
			snid = {$_REQUEST['snid']}
		WHERE
			dncpf = '{$_SESSION['usucpf']}'		
	";
	$db->executar($sqlUp);
	if($db->commit())
	{
		echo 'sucesso';
	}
	else
	{
		echo 'erro';
	}
	die();
}


function carregaMunicipios( $dados ){

	global $db;

	extract($dados);

	$sql = "SELECT muncod as codigo, mundescricao as descricao
	FROM territorios.municipio
	WHERE estuf = '$estuf'
	ORDER BY 2 ASC";
	$db->monta_combo( "muncod_combo", $sql, 'S', 'Todos municípios', '', '' );
	
}
if( $_REQUEST['requisicao'] == 'carregaMunicipios' ){
	echo carregaMunicipios( $_REQUEST );
	exit();
}

$cpfNutricionista = $_SESSION['usucpf'];
$situacaoOK = verificaVinculacaoNutricionista($cpfNutricionista);

if( ! $situacaoOK)
{
	echo "<script>alert('Para preencher os Dados Pessoais é necessário concluir o preenchimento do formulário na tela de Vinculação');
				window.location.href='par.php?modulo=principal/vinculacaoNutricionista&acao=A';
		  </script>";
	die();
}


// Salva o formulário
if($_POST['formulario'])
{
	
	$crnprovisorio = $_POST['dncrnprovisorio'] ? 't' : 'f';
	
	if($_POST['dnaceito'])
	{
		$aceito = ",dnaceito 				= '{$_POST['dnaceito']}'";
	}
	else
	{
		$aceito = "";
	}
	
	$db->executar(
		"
			UPDATE
				par.dadosnutricionista
			SET
				dncpf 					= '{$_POST['dncpf']}', 
				dnemailalternativo 		= '{$_POST['dnemailalternativo']}',
				dnnomemae 				= '{$_POST['dnnomemae']}',
				dncrnprovisorio			= '{$crnprovisorio}',
				dncrnuf					= '{$_POST['dncrnuf']}',
				dncrn 					= '{$_POST['dncrn']}'
				{$aceito}
			WHERE			
				entid 		= {$_POST['entid']}
			AND
				dncpf 		= '{$_POST['dncpf']}'
		"
	);
	
	
	$arrVnid 			= ($_POST['vnid']) ? $_POST['vnid'] : Array();
	$arrDataVinculacao 	= ($_POST['vndatavinculacao']) ? $_POST['vndatavinculacao'] : Array();
	$arrCargaHoraria	= ($_POST['vncargahorariasemanal']) ? $_POST['vncargahorariasemanal'] : Array();
	
	if(count($arrVnid) > 0)
	{
		foreach($arrVnid as $k => $vnid)
		{
			
			$arrDt = explode("/", $arrDataVinculacao[$k]);
			$dataVinculacao = $arrDt[2].'-'.$arrDt[1].'-'.$arrDt[0];
			
			$sqlVu = "
				UPDATE
					par.vinculacaonutricionista 
				SET
					vndatavinculacao = '{$dataVinculacao}',
					vncargahorariasemanal = {$arrCargaHoraria[$k]}
				WHERE
					vnid =	{$vnid}			
			";
			$db->executar($sqlVu);
		}
	}
	
	
	$arrDt = explode("/", $_POST['entdatanasc']); 
	$dataNascimento = $arrDt[2].'-'.$arrDt[1].'-'.$arrDt[0];
	
	
	$db->executar(
		"
			UPDATE
				entidade.entidade
			SET
				entsexo 					= '{$_POST['entsexo']}',
				entemail 					= '{$_POST['entemail']}',
				entdatanasc 				= '{$dataNascimento}',
				entnumdddresidencial		= '{$_POST['entnumdddresidencial']}',
				entnumresidencial			= '{$_POST['entnumresidencial']}',
				entnumdddcelular			= '{$_POST['entnumdddcelular']}',
				entnumcelular				= '{$_POST['entnumcelular']}'
			WHERE			
				entid 		= {$_POST['entid']}
		"
	);
	
	$existeEnd = $db->pegaUm( "SELECT entid FROM entidade.endereco WHERE entid = {$_POST['entid']} ");
	$cep = str_replace('-', '', $_POST['endcep']);
	if($existeEnd)
	{
		$db->executar(
				"
				UPDATE
					entidade.endereco
				SET
					endcep 						= '{$cep}',
					endlog 						= '{$_POST['endlog']}',
					endcom						= '{$_POST['endcom']}',
					endnum						= '{$_POST['endnum']}',
					endbai						= '{$_POST['endbai']}',
					estuf						= '{$_POST['estuf']}',
					muncod						=  {$_POST['muncod']}
				WHERE
					entid 		= {$_POST['entid']}
				"
			);
		
	}
	else
	{	
		$db->executar(
				"
				INSERT INTO
					entidade.endereco
				(
					endcep,
					endlog,
					endcom,
					endnum,
					endbai,
					estuf,
					muncod,
					entid
				)
				VALUES
				(	
					'{$cep}',
					'{$_POST['endlog']}',
					'{$_POST['endcom']}',
					'{$_POST['endnum']}',
					'{$_POST['endbai']}',
					'{$_POST['estuf']}',
					'{$_POST['muncod']}',
					{$_POST['entid']}
				)
				"
		);
	}
	
	$entidadeUsu = $db->PegaUm("select entid from seguranca.usuario where usucpf = '{$_POST['dncpf']}' ");
	
	//  insere dados do usuário
	$arrApelido = explode(" ", $_POST['nome']);
	$apelido = $arrApelido[0];
	
	
	if( $_POST['muncod'] == '5300108' )
	{
		$entidClause = " entid =  381942";
	}
	else
	{
		$entidClause = " entid = (
						select e.entid from
							entidade.entidade e
						INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
						LEFT  JOIN entidade.endereco ed ON ed.entid = e.entid 
						where funid = 1
						AND muncod = '{$_POST['muncod']}'
						AND entstatus = 'A'
						ORDER BY e.entid desc
						limit 1
					)";
	}
	
	if( ! $entidadeUsu )
	{
		$sql = "UPDATE 
				seguranca.usuario
			SET
				usunomeguerra = '{$apelido}',
				ususexo = '{$_POST['entsexo']}',
				regcod = '{$_POST['estuf']}',
				muncod = '{$_POST['muncod']}',
				tpocod = 3,
				usufoneddd = {$_POST['entnumdddresidencial']},
				usufonenum = {$_POST['entnumresidencial']},
				{$entidClause}
				,usudatanascimento		= '{$dataNascimento}'
			WHERE usucpf = '{$_POST['dncpf']}'";
		$db->executar($sql);
	} 
	
	$sqlVinculacoes = " select vnid from par.vinculacaonutricionista where vncpf =  '{$_POST['dncpf']}' AND snid = 6";
	$result = $db->carregar($sqlVinculacoes);
	$result = (is_array($result)) ? $result : Array();
	if(count($result) > 0 )
	{
		foreach( $result as $k => $v )
		{
			$resultVnid[] = $v['vnid'];
		}
		$vnidClause = implode(",", $resultVnid);
	
		$sqlUp = "UPDATE par.vinculacaonutricionista SET snid = 1 WHERE vnid IN ( {$vnidClause} )";
		
		$db->executar($sqlUp);
	}
	
	if($db->commit())
	{
		echo "<script>alert('Dados salvos com sucesso');
			   window.location.href='par.php?modulo=principal/cadastroNutricionista&acao=A';
			 </script>";
	}
	else
	{
		echo "<script>alert('Erro ao salvar os dados');
				window.location.href='par.php?modulo=principal/cadastroNutricionista&acao=A';
				</script>";
	}

}

if( $_REQUEST['popup'] == 'S' ){
	$cpfNutricionista = $_REQUEST['cpf'];
	?>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
	<script type="text/javascript" src="/includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<?php 
} else {
	require_once APPRAIZ . 'includes/cabecalho.inc';
	$cpfNutricionista = $_SESSION['usucpf'];
}
monta_titulo( "Cadastro Nutricionista", "Alimentação Escolar" );



$dadosNutri = 
	$db->pegaLinha("
	SELECT 
		dn.entid, 
		dncpf,
		entnome,
		entemail,
		ent.entnumrg, 				-- RG
		ent.entorgaoexpedidor, 		-- ORGAO
		CASE WHEN 
				ent.entsexo = 'M'
		THEN
				'masculino'
		WHEN 
				ent.entsexo = 'F'
		THEN 
				'feminino'	
		ELSE
				null
			
		END as entsexo, 				-- sexo (1)
		ent.entdatanasc, 			-- data nascimento
		ent.entnumdddresidencial, 	-- ddresidencial
		ent.entnumresidencial,		-- numeroresidencial
		ent.entnumdddcelular,		-- ddd celular
		ent.entnumcelular,			-- numero celular
		dn.dncrnprovisorio,			-- CRN PROVISORIO
		e.endcep,					-- CEP
		e.endlog,					-- Logradouro
		e.endcom,					-- Complemento
		e.endnum,					-- Número
		e.endbai,					-- Bairro
		e.estuf,					-- Estado
		e.muncod,					-- código município
		mun.mundescricao, 			-- múnicipio 
		dn.dncrnuf,					-- dncrnuf
		dn.dnnomemae,
		dn.dnemailalternativo,
		dn.dncrn
	FROM 
		par.dadosnutricionista  dn
	INNER JOIN entidade.entidade 	 ent 	ON ent.entid = dn.entid
	LEFT JOIN entidade.endereco 	 e 		ON e.entid = ent.entid
	LEFT JOIN territorios.municipio mun 	ON mun.muncod = e.muncod
	
	WHERE
		 dncpf = '{$cpfNutricionista}'		
	");

$unidades  = $db->carregar("
	select
		CASE WHEN inu.estuf IS NULL THEN
			'Prefeitura Municipal de ' || mun.mundescricao || ' - ' || mun.estuf  
		ELSE
			'Secretaria Estadual de Educação de ' || est.estdescricao
		END
			as unidade
		from par.dadosunidade du
		INNER JOIN par.instrumentounidade inu ON inu.inuid = du.inuid
		LEFT JOIN territorios.municipio mun ON mun.muncod = inu.muncod
		LEFT JOIN territorios.estado est ON est.estuf = inu.estuf

where duncpf  =	'{$cpfNutricionista}'	AND du.dutid in (11,12)
");

$unidades = ($unidades) ? $unidades : Array();
foreach($unidades as $k => $v)
{
	$arrunidades[] =  $v['unidade'];
}

$arrunidades = ($arrunidades) ? $arrunidades : Array();
$unidades = implode($arrunidades, '<br>');

$isTecnico  = $db->pegaUm("
	SELECT du.duncpf FROM PAR.dadosunidade du
	INNER JOIN 	par.vinculacaonutricionista vn ON vn.vncpf = du.duncpf and vnstatus  = 'A'
	WHERE duncpf = '{$cpfNutricionista}'	 AND vn.dutid = 11
");
$tecnico = 'false';

if($isTecnico)
{
	$tecnico = 'true';
}


?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<!-- JAVASCRIPT -->
<script>


function salvar()
{

	
	boSubmeter = true;
  	jQuery('form#formulario :input').each(function() {
		
	    if( jQuery(this).attr('type') == 'text' )
	    {
 		    if( jQuery(this).attr('name') != 'endcom' )
 		    {
			    if( jQuery(this).val() == '')
			    {
					alert("O campo '" + jQuery(this).attr('title') + "' tem preenchimento obrigatório" );
					jQuery(this).focus();
					boSubmeter = false;
					return false;
				}
 		    }
		    if( jQuery(this).attr('name') == 'entemail' )
		    {
		    	if(!validaEmail(jQuery(this).val())){
					alert('E-mail Principal inválido.');
					jQuery(this).focus();
					boSubmeter = false;
					return false;
				}
		    
		    } 
		    if( jQuery(this).attr('name') == 'dnemailalternativo' )
		    {
		    	if(!validaEmail(jQuery(this).val())){
					alert('E-mail Alternativo inválido.');
					jQuery(this).focus();
					boSubmeter = false;
					return false;
				}
		    
		    } 
		    if( jQuery(this).attr('name') == 'entdatanasc' )
		    {
		    	if(!validaData(document.getElementById('entdatanasc'))){
					alert('O campo Data de nascimento deve ser no formato dd/mm/YYYY.');
					jQuery('#entdatanasc').focus();
					boSubmeter = false;
					return false;
				}
		    
		    } 
	    }
	    else if( jQuery(this).attr('name') == 'dncrnuf' )
	    {
	    	if( jQuery(this).val() == '')
		    {
	    		alert("O campo 'Região do CRN' tem preenchimento obrigatório" );
	    		boSubmeter = false;
	    		return false; 
	    	}
	    }
	    else if( jQuery(this).attr('id') == 'entsexo' )
	    {
	    	if( ! (jQuery('[name=entsexo]').is(':checked')))
	    	{
	    		alert("O campo 'Sexo' tem preenchimento obrigatório" );
	    		boSubmeter = false;
	    		return false; 
	    	}
	    }
	    
	    else if( jQuery(this).attr('id') == 'dnaceito' )
	    {
	    	if( ! (jQuery('#dnaceito').is(':checked')))
	    	{
	    		alert("Para submeter o formulário é necessário clicar em 'Aceito' no campo 'Responsável Técnico' " );
	    		boSubmeter = false;
	    		return false; 
	    	}
	    }
	    
	    
	});
	
  	if(jQuery('#muncod1').val() == '' )
	{
		alert('Cep inexistente. Insira um CEP válido');
		boSubmeter = false;
		return false;
	}
	
	if(boSubmeter)
	{
		jQuery('#formulario').submit();
	}
}


jQuery(document).ready(function(){

	var popup = jQuery('[name="popup"]').val();

	jQuery('[name=endcep]').val(mascaraglobal('#####-###',jQuery('[name=endcep]').val()));			
	jQuery('[name=cpf]').val(mascaraglobal('###.###.###-##',jQuery('[name=cpf]').val()));			
	
	jQuery('[name="estuf_combo"]').change(function(){
		if( jQuery('[name="esfera"]').val() != 'E' ){
			if( jQuery(this).val() != '' ){
				jQuery('#aguardando').show();
				jQuery.ajax({
			   		type: "POST",
			   		url: window.location.href,
			   		data: "requisicao=carregaMunicipios&estuf="+jQuery(this).val(),
			   		async: false,
			   		success: function(resp){
			   			jQuery('#td_muncod').html(resp);
			   			jQuery('#aguardando').hide();
			   		}
			 	});
			}else{
				jQuery('#td_muncod').html('Escolha uma UF.');
			}
		}
	});
	
	jQuery('#endcep1').blur(function(){

		if( jQuery(this).val().length == 9 ){
	
			var endcep = jQuery(this).val();
			jQuery('#muncod1').val('');
			jQuery.ajax({
		        type: 'post',
		        url:  '/geral/consultadadosentidade.php',
		        data: 'requisicao=pegarenderecoPorCEP&endcep=' + endcep,
		        async: false,
		        success: function (res){
			        
		        	var dados = res.split("||");
		        	jQuery('#endlog1').val(dados[0]);
					jQuery('#endbai1').val(dados[1]);
					jQuery('#mundescricao1').val(dados[2]);
					jQuery('#estuf1').val(dados[3]);
					if(! dados[4])
					{
						alert('CEP Não encontrado. Selecione o Estado e Município');
						jQuery( "#dialog_municipio" ).show();
						
						jQuery( '#dialog_municipio' ).dialog({
								resizable: false,
								width: 450,
								dialogClass: "no-close",
								modal: true,
								
								open: function(){
 						            jQuery('.ui-widget-overlay').bind('click',function(){

 						            	comboVal = jQuery("[name='muncod_combo']").val();

 						            	if((comboVal == "") || ( ! comboVal) )
 						            	{
											alert('Preencha o Campo "Município"');
 						            	}
 						            	else
 						            	{
 						            		jQuery('#muncod1').val(jQuery("[name='muncod_combo']").val());
 						            		jQuery('#estuf1').val(jQuery("[name='estuf_combo']").val());
 						            		jQuery('#mundescricao1').val(jQuery("[name='muncod_combo'] option:selected").text());
 						            		jQuery('#dialog_municipio').dialog('close');
 						            	}
						            })
						    	},
								show: { effect: 'drop', direction: "up" },
								buttons: {
									
										'OK': function() {
										
										comboVal = jQuery("[name='muncod_combo']").val();

 						            	if((comboVal == "") || ( ! comboVal) )
 						            	{
											alert('Preencha o Campo "Município"');
 						            	}
 						            	else
 						            	{
 						            		jQuery('#muncod1').val(jQuery("[name='muncod_combo']").val());
 						            		jQuery('#estuf1').val(jQuery("[name='estuf_combo']").val());
 						            		jQuery('#mundescricao1').val(jQuery("[name='muncod_combo'] option:selected").text());
 						            		jQuery('#dialog_municipio').dialog('close');
 						            	}
									}
								}
						});						
					}
					else
					{
						jQuery('#muncod1').val(dados[4]);
					}
				}
			});
		}
	});
});






</script>


<?php

$arrAbas = array(
	0 => array("descricao" => "Vinculação", "link" 	=> "par.php?modulo=principal/vinculacaoNutricionista&acao=A"),
	1 => array("descricao" => "Dados pessoais", "link" 	=> "par.php?modulo=principal/cadastroNutricionista&acao=A"),
	2 => array("descricao" => "Desvinculação", "link" => "par.php?modulo=principal/desvinculaNutricionista&acao=A")
);


echo montarAbasArray($arrAbas, "par.php?modulo=principal/cadastroNutricionista&acao=A");
?>
<div id="formulario_validado" >
 <input type="hidden" id="snidVal" value="<?php echo $dadosNutri['snid'] ?>"/>
 <form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="popup" value="<?php echo $_REQUEST['popup'] ?>"/>
	<input type="hidden" name="entid" value="<?php echo $dadosNutri['entid'] ?>"/>
	<input type="hidden" name="tecnico" value="<?php echo $tecnico ?>"/>
	<input type="hidden" name="dncpf" value="<?php echo $cpfNutricionista; ?>"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	       	<td align='right' class="SubTituloDireita">CPF:</td>
	        <td>
				<?php echo campo_texto('cpf', 'S', 'N', 'CPF', 20, 15, '###.###.###-##', '',null,null,null,null,null, $cpfNutricionista); ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Nome:</td>
	        <td>
				<?php echo campo_texto('nome', 'S', 'N', 'Nome', 50, 50, '', '',null,null,null,null,null, $dadosNutri['entnome']); ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Data de nascimento:</td>
	        <td>
				<input type="text" name="entdatanasc" title="Data de nascimento"  id="entdatanasc" size="12" maxlength="10" value="<?=formata_data($dadosNutri['entdatanasc']) ?>" class="normal" onkeyup="this.value=mascaraglobal('##/##/####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);">&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Nome da mãe:</td>
	        <td>
	        
				<?php echo campo_texto('dnnomemae', 'S', 'S', 'Nome da mãe', 50, 50, '', '',null,null,null,null,null, $dadosNutri['dnnomemae']); ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Sexo:</td>
	        <td>
			
				<input type="radio" value="M" title="Sexo"  <?php echo ($dadosNutri['entsexo'] == 'masculino') ? "checked=checked" : ""; ?>  name="entsexo"  id="entsexo" 	>Masculino &nbsp; 
				<input type="radio" value="F" title="Sexo"  <?php echo ($dadosNutri['entsexo'] == 'feminino') ? "checked=checked" : ""; ?>  name="entsexo"  id="entsexo"  >Feminino &nbsp;  &nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">CRN/Região:</td>
	        <td>
				<?php echo campo_texto('dncrn', 'S', 'S', 'CRN/Região', 12, 12, '', '',null,null,null,null,null, $dadosNutri['dncrn']); ?>
				<br><?php
					$sql = "select distinct estuf as codigo, estuf as descricao from territorios.estado order by estuf";
					$db->monta_combo( "dncrnuf", $sql, 'S', 'Selecione', 'Tipo de vínculo', '', '','','', '',  false, $dadosNutri['dncrnuf'] );
				
				?>
				&nbsp; <input type="checkbox" name="dncrnprovisorio" <?php echo ($dadosNutri['dncrnprovisorio'] == 't') ? "checked=checked" : ""; ?> id="dncrnprovisorio" value="t" > Provisório
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">E-mail principal:</td>
	        <td>
				<?php echo campo_texto('entemail', 'S', 'S', 'E-mail principal', 50, 50, '', '',null,null,null,null,null, $dadosNutri['entemail']); ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">E-mail alternativo:</td>
	        <td>
				<?php echo campo_texto('dnemailalternativo', 'S', 'S', 'Email Alternativo', 50, 50, '', '',null,null,null,null,null, $dadosNutri['dnemailalternativo']); ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Telefone fixo:</td>
	        <td>
				(<input type="text" name="entnumdddresidencial" id="entnumdddresidencial" title="DDD Telefone fixo" size="3" maxlength="2"value="<?=$dadosNutri['entnumdddresidencial'] ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) 
				<input type="text" name="entnumresidencial" id="entnumresidencial" title="Telefone fixo" size="11" maxlength="9" value="<?=$dadosNutri['entnumresidencial'] ?>" class="normal" onkeyup="this.value=mascaraglobal('#########',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);">
				 &nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Telefone Celular:</td>
	        <td>
				(<input type="text" name="entnumdddcelular" id="entnumdddcelular" size="3" title="DDD Telefone celular" maxlength="2"value="<?=$dadosNutri['entnumdddcelular'] ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) 
				<input type="text" name="entnumcelular" id="entnumcelular" size="11" title="Telefone celular" maxlength="9" value="<?=$dadosNutri['entnumcelular'] ?>" class="normal" onkeyup="this.value=mascaraglobal('#########',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);">
				 &nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">CEP:</td>
	        <td>
				<input type="text" name="endcep" id="endcep1" title="CEP" value="<?=$dadosNutri['endcep'] ?>" size="11" maxlength="9" class="normal" onkeyup="this.value=mascaraglobal('#####-###',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);getEnderecoPeloCEP(this.value,'1');">
				
				 &nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        	</td>
      	</tr>
      	<tr id="tr_endlog">
      		<td class="SubTituloDireita">Logradouro :</td>
      		<td>
      			<input type="text" name="endlog" id="endlog1" title="Logradouro" value="<?=$dadosNutri['endlog'] ?>" size="80" maxlength="200" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);">
      			&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
      		</td>
      	</tr>
		
		<tr id="tr_endcom">
			<td class="SubTituloDireita">Complemento :</td>
			<td>
				<input type="text" name="endcom" id="endcom1" title="Complemento" value="<?=$dadosNutri['endcom'] ?>" size="80" maxlength="200" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);">
			</td>
		</tr>
		
		<tr id="tr_endnum">
			<td class="SubTituloDireita">Número :</td>
			<td>
				<input type="text" name="endnum" id="endnum1" title="Número" value="<?=$dadosNutri['endnum'] ?>" size="11" maxlength="10" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);">
				&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		
		<tr id="tr_endbai">
			<td class="SubTituloDireita">Bairro :</td>
			<td>
				<input type="text" name="endbai" id="endbai1" title="Bairro" value="<?=$dadosNutri['endbai'] ?>" size="80" maxlength="100" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);">
				&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		
		<tr id="tr_estuf">
			<td class="SubTituloDireita">UF :</td>
			<td>
				<input type="text" name="estuf" title="UF" id="estuf1" value="<?=$dadosNutri['estuf'] ?>" size="3" maxlength="2" class="normal">
<!-- 				&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> -->
			</td>
		</tr>
		<tr id="tr_mundescricao">
			<td class="SubTituloDireita">Munícipio :</td>
			<td>
				<input type="text" id="mundescricao1" title="Municipio" name="mundescricao1" value="<?=$dadosNutri['mundescricao'] ?>" size="30" class="normal">
				&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				<input type="hidden" name="muncod" id="muncod1" value="<?=$dadosNutri['muncod'] ?>"> 
			</td>
		</tr>

		<tr id="tr_acoes">
			<td class="SubTituloCentro" colspan="2">
			<input type="button" value="Finalizar Cadastro" id="btngravar" onclick="salvar()">
				
		</td>
		
		
	</table>
	
	
 </form>
 
 
</div>
<style>
.no-close .ui-dialog-titlebar-close {
  display: none;
}
</style>
<div id="dialog_municipio" title="Selecione seu Estado e Município" style="display: none; text-align: center;">
		<center><table>
		 <tr>
			<td class="subtituloDireita">UF:</td>
			<td colspan="3">
				<?php
					$estuf = $_POST['estuf_combo'];
					$sql = "SELECT e.estuf as codigo, e.estdescricao as descricao FROM territorios.estado e ORDER BY e.estdescricao ASC";
					$db->monta_combo( "estuf_combo", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="subtituloDireita"> Município:</td>
			<td id="td_muncod" colspan="3">
				<?php
					if( $estuf ){
						$muncod = $_POST['muncod'];
						$sql = "SELECT muncod as codigo, mundescricao as descricao 
								FROM territorios.municipio 
								WHERE estuf = '$estuf'
								ORDER BY 2 ASC";
						$db->monta_combo( "muncod_combo", $sql, 'S', 'Todos municípios', '', '' );
					}else{
						echo "Escolha uma UF.";
					}
				?>
			</td>
		</tr>	
		</table>
		</center>						
</div>