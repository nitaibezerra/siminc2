<?php

	$cpfNutricionista = $_REQUEST['cpf'];
	?>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
	<script type="text/javascript" src="/includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<?php 

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

where duncpf  =	'{$cpfNutricionista}'	
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



$sqlVinculacaoUnidade = "
	SELECT 
		
		CASE WHEN inu.estuf IS NULL THEN
			m.mundescricao || '-' || inu.mun_estuf
		ELSE
			estdescricao
		END
			as unidade,
		
		CASE WHEN vnstatus = 'A' THEN
			'<center><img border=\"0\" title=\"\" src=\"../imagens/check_checklist.png\" width=\"15px\"></center>'
		ELSE
			''
		END as ativo,
		
		CASE WHEN vnstatus = 'I' THEN
			'<center><img border=\"0\" title=\"\" src=\"../imagens/check_checklist.png\"  width=\"15px\"></center>'
		ELSE
			''
		END as inativo,
		
	
		CASE WHEN vn.vndatavinculacao IS NOT NULL THEN to_char(vn.vndatavinculacao,'DD/MM/YYYY') ELSE '' END as vndatavinculacao,
		vn.vncargahorariasemanal::text as vncargahorariasemanal,
		
		
		to_char(vn.vndatadesvinculacao,'DD/MM/YYYY') as vndatadesvinculacao,
		dntvdescricao,

		CASE WHEN vnatuacaoexclusivaei = false THEN
			'Não'
		WHEN vnatuacaoexclusivaei = true THEN
			'Sim'
		ELSE
			''
		END as atuacao_exclusiva,
		
		CASE WHEN snaceito = true THEN
			'<center><img border=\"0\" title=\"\" src=\"../imagens/check_checklist.png\" width=\"15px\"></center>'
		ELSE
			''
		END as sn_aceito

		
	FROM
		par.vinculacaonutricionista  vn
	INNER JOIN par.instrumentounidade inu ON inu.inuid = vn.inuid
	LEFT JOIN par.dadosunidade  	du ON du.duncpf = vn.vncpf AND vn.inuid = du.inuid and vn.dutid = du.dutid
	LEFT JOIN par.dadosnutricionistatipovinculo dnt ON vn.dntvid = dnt.dntvid
	LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
	LEFT JOIN territorios.estado e ON e.estuf = inu.estuf
	
	WHERE
		vncpf =  '{$cpfNutricionista}'
	
	AND
		CASE WHEN du.dunid IS NOT NULL
		THEN
			du.dutid in (11,12)
		ELSE
			true
		END	
";






?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<!-- JAVASCRIPT -->
<script>



jQuery(document).ready(function(){


	var snid = jQuery('#snidVal').val();
	var popup = jQuery('[name="popup"]').val();
	
	jQuery('[name=cpf]').val(mascaraglobal('###.###.###-##',jQuery('[name=cpf]').val()));			
		
	jQuery('#endcep1').blur(function(){

		if( jQuery(this).val().length == 9 ){
	
			var endcep = jQuery(this).val();
			
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
						alert('Cep inexistente. Insira um CEP válido');
						jQuery('#muncod1').val('');
					}
					jQuery('#muncod1').val(dados[4]);
				}
			});
		}
	});
});





</script>

 <input type="hidden" id="snidVal" value="<?php echo $dadosNutri['snid'] ?>"/>
 <form action="" method="post" name="formulario" id="formulario">
 
 <?php if
 
 ( ! $dadosNutri['entdatanasc']  )
 {
 	
 	monta_titulo( "", "<span style='color:red'> Os dados ainda não estão disponíveis, o nutricionista ainda não salvou as informações. </span>" );
 	die();
 	?>
 		
 	<?php 
 }
 	
 	?>
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="popup" value="<?php echo $_REQUEST['popup'] ?>"/>
	<input type="hidden" name="entid" value="<?php echo $dadosNutri['entid'] ?>"/>
	<input type="hidden" name="tecnico" value="<?php echo $tecnico ?>"/>
	<input type="hidden" name="dncpf" value="<?php echo $cpfNutricionista; ?>"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	       	<td align='right' class="SubTituloDireita">CPF:</td>
	        <td>
				<?php echo campo_texto('cpf', 'N', 'N', 'CPF', 20, 15, '###.###.###-##', '',null,null,null,null,null, $cpfNutricionista); ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Nome:</td>
	        <td>
				<?php echo $dadosNutri['entnome']; ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Data de nascimento:</td>
	        <td>
				<?=formata_data($dadosNutri['entdatanasc']) ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Nome da mãe:</td>
	        <td>
	        
				<?php echo $dadosNutri['dnnomemae']; ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Sexo:</td>
	        <td>
			
				<?php echo ($dadosNutri['entsexo'] == 'masculino') ? "Masculino" : "Feminino"; ?> 
				 
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">CRN/Região:</td>
	        <td>
				<?php echo $dadosNutri['dncrn']; ?>
				/<?php
					echo $dadosNutri['dncrnuf'];
				
				?>
				<?php echo ($dadosNutri['dncrnprovisorio'] == 't') ? "<span style='color:red'>*Provisório</>" : ""; ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">E-mail principal:</td>
	        <td>
				<?php echo $dadosNutri['entemail']; ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">E-mail alternativo:</td>
	        <td>
				<?php echo $dadosNutri['dnemailalternativo']; ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Telefone fixo:</td>
	        <td>
				(<?=$dadosNutri['entnumdddresidencial']?>)  
				<?=$dadosNutri['entnumresidencial'] ?>
				 
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Telefone Celular:</td>
	        <td>
				(<?=$dadosNutri['entnumdddcelular'] ?>) 
				<?=$dadosNutri['entnumcelular'] ?>
				 
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">CEP:</td>
	        <td>
				<?=$dadosNutri['endcep'] ?>
        	</td>
      	</tr>
      	<tr id="tr_endlog">
      		<td class="SubTituloDireita">Logradouro :</td>
      		<td>
      			<?=$dadosNutri['endlog'] ?>
      		</td>
      	</tr>
		
		<tr id="tr_endcom">
			<td class="SubTituloDireita">Complemento :</td>
			<td>
				<?=$dadosNutri['endcom'] ?>
			</td>
		</tr>
		
		<tr id="tr_endnum">
			<td class="SubTituloDireita">Número :</td>
			<td>
				<?=$dadosNutri['endnum'] ?>
				
			</td>
		</tr>
		
		<tr id="tr_endbai">
			<td class="SubTituloDireita">Bairro :</td>
			<td>
				<?=$dadosNutri['endbai'] ?>
				
			</td>
		</tr>
		
		<tr id="tr_estuf">
			<td class="SubTituloDireita">UF :</td>
			<td>
				<?=$dadosNutri['estuf'] ?>

			</td>
		</tr>
		<tr id="tr_mundescricao">
			<td class="SubTituloDireita">Munícipio :</td>
			<td>
				<?=$dadosNutri['mundescricao'] ?>
				
			</td>
		</tr>
		<tr >
			<td class="SubTituloDireita"> Entidades vinculadas:</td>
			<td>
			<?php
				$db->monta_lista_simples($sqlVinculacaoUnidade, array('Entidade','Ativo','Inativo','Data de vinculação','Carga Horária Semanal','Data de desvinculação','Tipo de Vínculo', 'Atuação exclusiva na modalidade de Educação Infantil (creche e pré-escola)','Responsável Técnico (Aceitou o termo de responsabilidade técnica)' ), 25000, 10, 'N');
			?>
			</td>
		</tr>
		
		<tr id="tr_acoes">
			<td class="SubTituloCentro" colspan="2">
	<?php 	if( $_REQUEST['popup'] == 'S' ){?>
				<input type="button" value="Fechar" id="btnfechar" onclick="window.close();">
	<?php 	} else {?>
				<input type="button" value="Gravar" id="btngravar" onclick="salvar()">
	 <?php	}?>
				
		</td>
		
		
	</table>
	
	
 </form>

