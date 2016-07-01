<?php

if($_REQUEST['requisicao']== 'validaDataDesvinculacao')
{
	
	
	$arrDt = explode("/", $_REQUEST['data']);
	$dataDesvinculacao = $arrDt[2].'-'.$arrDt[1].'-'.$arrDt[0];
	
	$vnid = $_REQUEST['vnid'];
	$unidade = $_REQUEST['unidade'];
	
	
	$sql =
	"
		SELECT
			CASE 
			WHEN vndatavinculacao is null THEN
				'Preencha a data de vinculação da unidade {$unidade} na aba de \'vinculação\''
			
			WHEN vndatavinculacao > '{$dataDesvinculacao}' THEN
				'A data de desvinculação da unidade {$unidade} não pode ser inferior a data de vinculação (' || to_char(vndatavinculacao,'DD/MM/YYYY') || ')  ' 
		
			WHEN 
				'{$dataDesvinculacao}' > now() 
			THEN
				'A data de desvinculação da unidade {$unidade} não pode ser superior a data atual (' || to_char(now(),'DD/MM/YYYY') || ')'
			ELSE
				'sucesso'
			END
		FROM 
			par.vinculacaonutricionista
		WHERE
			vnid = $vnid 
	";
	
	$resposta = $db->pegaUm($sql);
	
	echo $resposta;
	die();
	
}

$cpfNutricionista = $_SESSION['usucpf'];
$situacaoOK = verificaVinculacaoNutricionista($cpfNutricionista);

if( ! $situacaoOK)
{
	echo "<script>alert('Para preencher os dados de Desvinculação é necessário concluir o preenchimento do formulário na tela de Vinculação');
				window.location.href='par.php?modulo=principal/vinculacaoNutricionista&acao=A';
		  </script>";
	die();
}

// Salva o formulário
if($_POST['formulario'])
{
	
	$arrVnid 			= ($_POST['vnid']) ? $_POST['vnid'] : Array();
	$selecionado 		= $_POST['radiovnid'];
	$arrDataDesvinculacao 	= ($_POST['vndatadesvinculacao']) ? $_POST['vndatadesvinculacao'] : Array();
	
	if(count($arrVnid) > 0)
	{
		foreach($arrVnid as $k => $vnid)
		{
			
			if( $vnid == $selecionado)
			{
				
				$arrDt = explode("/", $arrDataDesvinculacao[$k]);
				$dataDesvinculacao = $arrDt[2].'-'.$arrDt[1].'-'.$arrDt[0];
				
				$sqlVu = "
					UPDATE
						par.vinculacaonutricionista 
					SET
						vnstatus = 'I',
						vndatadesvinculacao = '{$dataDesvinculacao}'
						,usucpfalteracao = '{$_SESSION['usucpf']}'
						
					WHERE
						vnid =	{$vnid}
				";
				$db->executar($sqlVu);
			}
			
		}
	}
	
	if($db->commit())
	{
		echo "<script>alert('Dados salvos com sucesso');</script>";
	}
	else
	{
		echo "<script>alert('Erro ao salvar os dados');</script>";
	}

}


require_once APPRAIZ . 'includes/cabecalho.inc';
monta_titulo( "Termo de Desvinculação", "Alimentação Escolar" );

$cpfNutricionista = $_SESSION['usucpf'];

$dadosNutri = 
	$db->pegaLinha("
	SELECT 
		dn.entid
		
	FROM 
		par.dadosnutricionista  dn
	WHERE
		 dncpf = '{$cpfNutricionista}'		
	");

//@todo estadual
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
and du.dutid in (11,12)
");
$unidades = ($unidades) ? $unidades : Array();
foreach($unidades as $k => $v)
{
	$arrunidades[] =  $v['unidade'];
}

$arrunidades = ($arrunidades) ? $arrunidades : Array();

$unidades = implode($arrunidades, '<br>');

$isTecnico  = $db->pegaUm("
	SELECT * FROM PAR.dadosunidade WHERE duncpf = '{$cpfNutricionista}'	 AND dutid = 11
");
$tecnico = 'false';

if($isTecnico)
{
	$tecnico = 'true';
}



$sqlVinculacaoUnidade = "
	SELECT 
		DISTINCT
		CASE WHEN vnstatus = 'A' THEN
			'<center><input type=\'radio\' onClick=\"javascript:selecionaEntidade(this.value)\" name=\'radiovnid\' value=\'' || vn.vnid::text  || '\'></center>'  
		ELSE
			''
		END
		as acao,
		
		CASE WHEN inu.estuf IS NULL THEN
			m.mundescricao 
		ELSE
			'<center>-</center>'
		END
		
		||
		'<input type=\'hidden\' id=\'unidade'|| vn.vnid::text ||'\'
		value=\"
		'||
			
		CASE WHEN inu.estuf IS NULL THEN
			m.mundescricao || '-' || inu.mun_estuf
		ELSE
			estdescricao
		END
		
		||' \"
		
		\>'
		
			as municipio,
			
		CASE WHEN inu.estuf IS NULL THEN
			inu.mun_estuf
		ELSE
			inu.estuf
		END
			as estado,
		

		
		to_char(vn.vndatavinculacao,'DD/MM/YYYY') as vndatavinculacao,
		-- Data vinculacao
		CASE WHEN vnstatus = 'A' THEN
			'<div id=\"div_data'|| vn.vnid ||'\" style=\"display: none;\"> <input name=\'vndatadesvinculacao[]\'
				id=\"data_desvincula'|| vn.vnid::text ||'\"
				onkeyup=\"this.value=mascaraglobal(\'##/##/####\',this.value);\"
				value=\''
				|| 
					CASE WHEN vn.vndatadesvinculacao IS NOT NULL THEN to_char(vn.vndatadesvinculacao,'DD/MM/YYYY') ELSE '' END  
				||'\' 
				type=\'text\'
				title=\'Data Desvinculação - '
				|| 
						CASE WHEN inu.estuf IS NULL THEN	replace( m.mundescricao , '\'', '`')  || '-' || inu.mun_estuf ELSE estdescricao END
				||' \'
				
				'
				||
					
						'onBlur=\"javascript:validaDataDesvinculacao(
						'|| vn.vnid::text || ',
						 this.value ,
						\'' 
						||
							CASE WHEN inu.estuf IS NULL THEN
								replace( m.mundescricao , '\'', '`') || '-' || inu.mun_estuf
							ELSE
								estdescricao
							END
						||
						'\')\"'
					
				||
				'
			></div>' ||
			
			'<input 
				name=\'vnid[]\' 
				type=\'hidden\'
				value=\''
				|| 
					vnid::text  
				||'\'  
			>'
		ELSE
			'<center>' || to_char(vn.vndatadesvinculacao,'DD/MM/YYYY') || '</center>'
			||
			'<input 
				name=\'vnid[]\' 
				type=\'hidden\'
				value=\''
				|| 
					vnid::text  
				||'\'  
			>'
			||
			'<input 
				name=\'vndatadesvinculacao[]\' 
				type=\'hidden\'
				id=\"data_desvincula'|| vn.vnid::text ||'\"
				value=\''
				|| 
					to_char(vn.vndatadesvinculacao,'DD/MM/YYYY')
				||'\'  
			>'
		END	
			,
		
		
		CASE WHEN vnassinado = false THEN
			CASE WHEN vnstatus = 'A' THEN
				'<div id=\"div_termo'|| vn.vnid ||'\" style=\"display: none;\"> <center><input type=\"button\" value=\"Assinar Termo\" id=\"btngravar\" onclick=\"assinarTermoDesvinculacao('|| vn.vnid ||', \'ativo\', '|| du.dunid  ||' );\">  </center></div>'
			ELSE
				'<center><input type=\"button\" value=\"Assinar Termo\" id=\"btngravar\" onclick=\"assinarTermoDesvinculacao('|| vn.vnid ||',\''|| to_char(vn.vndatadesvinculacao,'DD/MM/YYYY') || '\', '|| du.dunid  ||' );\">  </center>'
			END
				
			
		ELSE
			'<center><img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"assinarTermoDesvinculacao('|| vn.vnid ||',\''|| to_char(vn.vndatadesvinculacao,'DD/MM/YYYY') || '\' , \'visu\' );\" /></center>' 
		END
	
		as download
	
	FROM
		par.vinculacaonutricionista  vn
	INNER JOIN par.instrumentounidade inu ON inu.inuid = vn.inuid
	LEFT JOIN par.dadosunidade  du		ON du.duncpf = vn.vncpf AND vn.inuid = du.inuid
	LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
	LEFT JOIN territorios.estado e ON e.estuf = inu.estuf
	
	WHERE
		vncpf = '{$cpfNutricionista}'
	AND
		CASE WHEN du.dunid IS NOT NULL
		THEN
			du.dutid in (11,12) OR vn.vnstatus = 'I'
		ELSE
			true
		END
	AND (vn.snid <> 2 OR vn.snid is null)
";


?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<!-- JAVASCRIPT -->
<script>

function assinarTermoDesvinculacao(vnid, data, dunid)
{
	if(data == 'ativo')
	{
	 	if( jQuery('#data_desvincula'+vnid).val() == '')
	 	{
			alert( "Preencha o campo \"Data de desvinculação\"");
	 	}
	 	else
	 	{

		 	var dataDesv = jQuery('#data_desvincula'+vnid).val();

		 	if(dunid != 'visu')
		 	{
				window.open('par.php?modulo=principal/popUpTermoDesvinculacao&acao=A&vnid=' + vnid + '&vndatadesvinculacao='+dataDesv + '&dunid='+dunid ,  '' ,'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);
		 	}
		 	else
		 	{
				window.open('par.php?modulo=principal/popUpTermoDesvinculacao&acao=A&vnid=' + vnid + '&vndatadesvinculacao='+dataDesv,  '' ,'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);
		 	}
	 	}
	}
	else
	{
		if(dunid != 'visu')
	 	{
			window.open('par.php?modulo=principal/popUpTermoDesvinculacao&acao=A&vnid=' + vnid + '&vndatadesvinculacao='+data + '&dunid='+dunid ,  '' ,'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);
	 	}
		else
		{
			window.open('par.php?modulo=principal/popUpTermoDesvinculacao&acao=A&vnid=' + vnid + '&vndatadesvinculacao='+data,  '' ,'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);
		}
	}	
}

function visualizaTermoDesvinculacao(vnid)
{
	alert('visualiza');
}

function selecionaEntidade(vnid)
{
		var selecionado = jQuery('input:radio[name=radiovnid]:checked').val();

		jQuery('form#formulario :radio').each(function() {

			idLoop = jQuery(this).val();
			if( idLoop == vnid )
			{
				jQuery('#div_termo'+vnid).show();
				jQuery('#div_data'+vnid).show();
			}
			else
			{	
				jQuery('#div_termo'+idLoop).hide();
				jQuery('#div_data'+idLoop).hide();
			}

		
		//alert();

	
	});
	//div_termo
	//div_data
	/*
	jQuery('#formulario_validado').show();
	jQuery('[name=cpf]').val(mascaraglobal('###.###.###-##',jQuery('[name=cpf]').val()));			

	jQuery('#formulario_validado').hide();
	*/
}



function validaDataDesvinculacao( id, data, unidade)
{

	//Se for técnico
	if(!validaData(document.getElementById('data_desvincula'+id))){
		alert('O campo "Data de vinculação" da entidade ' + unidade +' deve ser no formato dd/mm/YYYY.');
		var selecionado = jQuery('input:radio[name=radiovnid]:checked').val();
		jQuery('#data_desvincula'+selecionado).focus();
		jQuery('#data_desvincula'+selecionado).val('');
		return false;
	}
	
	jQuery.ajax({
        type: 'post',
        url:  'par.php?modulo=principal/desvinculaNutricionista&acao=A',
        data: 'requisicao=validaDataDesvinculacao&vnid=' + id + '&data='+ data + '&unidade='+unidade,
        async: false,
        success: function (res){

			if(res == 'sucesso')
			{
	        	return true;
			}
			else
			{
				alert(res);
				var selecionado = jQuery('input:radio[name=radiovnid]:checked').val();
				jQuery('#data_desvincula'+selecionado).focus();
				jQuery('#data_desvincula'+selecionado).val('');
				
			}
				
		}
	});


	return true;
}

function salvar()
{

	boSubmeter = true;

	var selecionado = jQuery('input:radio[name=radiovnid]:checked').val();

	if(!selecionado)
	{
		alert( "Selecione pelo menos uma entidade para desvincular");
		boSubmeter = false;
		return false;
	}
	
	if( jQuery('#data_desvincula'+selecionado).val() == '')
 	{
		alert( "Preencha o campo \"Data de desvinculação\"");
		boSubmeter = false;
		return false;
 	}
	
	if(boSubmeter)
	{
		if (  ( confirm( 'Você Confirma a desvinculação com a entidade ' + jQuery('#unidade'+selecionado).val() + 'na data ' + jQuery('#data_desvincula'+selecionado).val() ) )) 
		{
			jQuery('#formulario').submit();
		}
		
	}
}


jQuery(document).ready(function(){


	
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

<div id="formulario_validado" >
<?php 


$arrAbas = array(
	0 => array("descricao" => "Vinculação", "link" 	=> "par.php?modulo=principal/vinculacaoNutricionista&acao=A"),
	1 => array("descricao" => "Dados pessoais", "link" 	=> "par.php?modulo=principal/cadastroNutricionista&acao=A"),
	2 => array("descricao" => "Desvinculação", "link" => "par.php?modulo=principal/desvinculaNutricionista&acao=A")
);


echo montarAbasArray($arrAbas, "par.php?modulo=principal/desvinculaNutricionista&acao=A");
?>
 <input type="hidden" id="snidVal" value="<?php echo $dadosNutri['snid'] ?>"/>
  
 <form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="entid" value="<?php echo $dadosNutri['entid'] ?>"/>
	<input type="hidden" name="tecnico" value="<?php echo $tecnico ?>"/>
	<input type="hidden" name="dncpf" value="<?php echo $_SESSION['usucpf']; ?>"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	       	<td>
	       		<?php
	       		
				$db->monta_lista_simples($sqlVinculacaoUnidade, array('Ação','Município','UF','Data de vinculação','Data de desvinculação','Termo'), 25000, 10, 'N');
				?>
	       	</td>
      	</tr>
		<tr>
			<td>
			<center>
				
			</center>
			</td>
	    </tr>
		
	</table>
	
	
 </form>
 
 
</div>
