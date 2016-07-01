<?php

function getEmailRespNutriVinc($inuid)
{
	global $db;

	$sql = "
		SELECT iu.itrid,
		CASE WHEN iu.itrid = 2 THEN
			iu.muncod
		WHEN iu.itrid = 1 THEN
			iu.estuf
		END as filtro
		FROM
			par.instrumentounidade iu
		WHERE
			inuid = {$inuid}
	";

	$result = $db->pegaLinha($sql);
	$itrid = $result['itrid'];
	$filtro = $result['filtro'];


	if( ($itrid == 2) && ($filtro)  )
	{
	$sqlEmail = "SELECT
			ent.entemail as email
		FROM
			par.entidade ent
		INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 6   AND ent2.entstatus = 'A'
		INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
		WHERE
			ent.dutid =  7
			and ent.entstatus = 'A'
		AND
			mun.muncod in ( '{$filtro}' )
		";
		$esfera = 'Município';
	}
	else if( ($itrid == 1) && ($filtro))
	{
		$sqlEmail = "SELECT
			ent.entemail as email
		FROM
			par.entidade ent
		INNER JOIN par.entidade ent2 ON ent2.muncod = ent.muncod AND ent2.dutid = 9  AND ent2.entstatus = 'A'
		INNER JOIN territorios.estado est on est.estuf = ent2.estuf
	
		WHERE
			ent.entstatus='A'
		AND
			ent.dutid =  10
		AND
			ent2.estuf in ( '{$filtro}' )";
		$esfera = 'Estado';
	}



	$resultEmail = $db->pegalinha($sqlEmail);

	$emailTo =  $resultEmail['email'];
	if( ! $emailTo )
	{
		return false;
	}
	else
	{
	return array(
		'email' =>$emailTo,
		'esfera' => $esfera
		) ;
	}
}

// Requisicao Ajax para validação
if($_REQUEST['requisicao']== 'validacaoNutricionista')
{
	
	//Carrega variáveis
	$vnid		= $_POST['vnid'];
	$snid		= $_POST['snid'];
	$dunid		= $_POST['dunid'];
	
	// Verifica, caso esteja não validando ele irá inativar também
	if($snid == 2)
	{
		$vinculacaoStatus = "
				,vnstatus 	= 'I'
				,usucpfalteracao = '{$_SESSION['usucpf']}'
				,vndatadesvinculacao = 'now()' ";
	}
	else if($snid == 6) 
	{
		$crn = $db->pegaUm("select dncrn from par.dadosnutricionista  where dncpf = '{$_SESSION['usucpf']}'");
		
		if($crn != '')
		{
			$snid = 1;
		}
		$vinculacaoStatus = '';
	}
	else 
	{
		$vinculacaoStatus = '';
	}
	
	// Executa a mudança
	$sqlUp = 
	"
		UPDATE 
			par.vinculacaonutricionista
		SET
			snid = {$snid}
			{$vinculacaoStatus}
		WHERE
			vnid = '{$vnid}'	
			
	";
	
	$db->executar($sqlUp);
	
	// Caso ocorra tudo certo com a solicitação
	if($db->commit())
	{
		// Caso esteja inativando
		if($snid == 2)
		{
			// 
			$dadosNutri = $db->pegaLinha(
				"SELECT
					inuid, entnome  from par.vinculacaonutricionista  vn
				INNER JOIN par.dadosnutricionista dn ON dn.dncpf = vn.vncpf
				INNER JOIN entidade.entidade 	 ent 	ON ent.entid = dn.entid
				WHERE
					vnid = {$vnid}");
			$inuid 		= $dadosNutri['inuid'];
			$nomeNutri	= $dadosNutri['entnome'];
			
			$obDadosUnidade = new DadosUnidade($dunid);
			$dadosU = $obDadosUnidade->getDados();
			$dutid = $dadosU['dutid'];

			if($dutid == 11)
			{
				$cargo = "Responsável Técnico/Nutricionista";
			}
			else
			{
				$cargo = "Quadro Técnico/Nutricionistas";
			}
			
			$obDadosUnidade->excluir($dunid);
			
			if($obDadosUnidade->commit())
			{
				
				if($inuid)
				{
					$arrEmail 	= getEmailRespNutriVinc($inuid);
					
					if($arrEmail)
					{
						$email		= $arrEmail['email'];
						$esfera		= $arrEmail['esfera'];
					
						$strAssunto = "Aviso de desvinculação de nutricionista";
					
						$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
							
						$strMensagem =
						"
<pre align=\"center\" style=\"text-align: justify;\"  >
	Informamos que o(a) nutricionista {$nomeNutri} não validou o vínculo indicado pela entidade como  {$cargo}
no cadastro do Programa Nacional de Alimentação Escolar nesta data. Observe se o número de nutricionistas vinculados ao seu {$esfera} 
está de acordo com os parâmetros numéricos definidos na Resolução CFN nº 465/2010.

http://simec.mec.gov.br

Atenciosamente,
Equipe SIMEC/PAR.

</pre>";
						if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" )
						{
							echo 'sucesso';
							die();
						}
						elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
						{
							$strEmailTo = array($_SESSION['email_sistema']);
							$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
							echo 'sucesso';
							die();
						}
						else
						{
							$strEmailTo = $email;
							$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
							echo 'sucesso';
							die();
						}
						
					}
					else
					{
						echo 'Erro ao enviar Email';
						die();
					}
					
				}
				else
				{
					echo 'Erro ao enviar Email';
					die();
				}
				
				
			}
			else
			{
				echo 'Erro ao excluir vinculação do Nutricionista';
				die();
			}
		}
		else
		{
			echo 'sucesso';
			die();
		}
	
	}
	else
	{
		echo 'Erro ao setar validação';
		die();
	}
	die();
}

// Salva o formulário
if($_POST['formulario'])
{
	$arrVnid 			= ($_POST['vnid']) ? $_POST['vnid'] : Array();
	$arrDataVinculacao 	= ($_POST['vndatavinculacao']) ? $_POST['vndatavinculacao'] : Array();
	$arrCargaHoraria	= ($_POST['vncargahorariasemanal']) ? $_POST['vncargahorariasemanal'] : Array();
	$arrDntvid			= ($_POST['dntvid']) ? $_POST['dntvid'] : Array();
	
	
	
	if(count($arrVnid) > 0)
	{
		foreach($arrVnid as $k => $vnid)
		{
			$arrDt = explode("/", $arrDataVinculacao[$k]);
			$dataVinculacao = $arrDt[2].'-'.$arrDt[1].'-'.$arrDt[0];
			
			if(strlen($dataVinculacao) < 10 )
			{
				echo "<script>alert('Erro ao salvar. Data inválida.');
					window.location.href='par.php?modulo=principal/vinculacaoNutricionista&acao=A';
				</script>";
			}
			
			$atuacaoexclusiva = $_POST['vnatuacaoexclusivaei_'.$vnid];
			if($atuacaoexclusiva)
			{
				$vnatuacaoexclusivaei = ",vnatuacaoexclusivaei = '{$atuacaoexclusiva}'";
			}
			else
			{
				$vnatuacaoexclusivaei  = "";
			}
			
			if($_POST['hsnaceito_'.$vnid] == "check")
			{
				$snaceito = ",snaceito = true";
			}
			else
			{
				$snaceito = "";
			}
			
			$sqlVu = "
				UPDATE
					par.vinculacaonutricionista 
				SET
					vndatavinculacao = '{$dataVinculacao}',
					vncargahorariasemanal = {$arrCargaHoraria[$k]},
					dntvid			= {$arrDntvid[$k]}
					{$snaceito}
					{$vnatuacaoexclusivaei}
				WHERE
					vnid =	{$vnid}			
			";
			
			$db->executar($sqlVu);
		}
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
				window.location.href='par.php?modulo=principal/vinculacaoNutricionista&acao=A';
				
			  </script>";
	}

}


	require_once APPRAIZ . 'includes/cabecalho.inc';
	$cpfNutricionista = $_SESSION['usucpf'];

	monta_titulo( "Vinculação Nutricionista", "Alimentação Escolar" );


//Recupera dos dados do nutricionista
$dadosNutri = 
	$db->pegaLinha("
	SELECT 
		dn.entid, 
		dncpf,
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
		vnstatus,
		CASE WHEN vnstatus = 'A' THEN
			'<center><img border=\"0\" title=\"Aguardando\" src=\"../imagens/check_checklist.png\" width=\"15px\"></center>'
		ELSE
			''
		END as ativo,
		
		CASE WHEN vnstatus = 'I' THEN
			'<center><img border=\"0\" title=\"Aguardando\" src=\"../imagens/check_checklist.png\"  width=\"15px\"></center>'
		ELSE
			''
		END as inativo,
		
		CASE WHEN vnstatus = 'A' THEN
			-- Data vinculacao
			'<input name=\'vndatavinculacao[]\'
			
				'||
				
					CASE WHEN vn.snid = 2 OR vn.snid = 3 THEN
						'disabled=\"disabled\"'
					ELSE
						''
					END
				
				||'
				id=\"data_vincula_'|| vn.vnid::text ||'\"
				onkeyup=\"this.value=mascaraglobal(\'##/##/####\',this.value);\"
				value=\''
				|| 
					CASE WHEN vn.vndatavinculacao IS NOT NULL THEN to_char(vn.vndatavinculacao,'DD/MM/YYYY') ELSE '' END  
				||'\' 
				type=\'text\'
				title=\'Data Vinculação - '
				|| 
						CASE WHEN inu.estuf IS NULL THEN	 replace( m.mundescricao , '\'', '`') || '-' || inu.mun_estuf ELSE estdescricao END
				||' \'
				
				'
				||
					
 						'onBlur=\"javascript:carregaTextoVinculacao(
						'|| vn.dutid::text || ',
						'|| vn.vnid::text || ',
						 this.value ,
						\'' 
						||
							CASE WHEN inu.estuf IS NULL THEN
								'Município' || ' ' || replace( m.mundescricao , '\'', '`') || '-' || inu.mun_estuf
							ELSE
								'Estado' || ' ' || replace( estdescricao, '\'', '`')
							END
						||
						'\')\"'
					
				||
				'
			>' 
				
			||
			
			'<input 
				name=\'vnid[]\' 
				type=\'hidden\'
				value=\''
				|| 
					vnid::text  
				||'\'  
				'||
				
					CASE WHEN vn.snid = 2 OR vn.snid = 3 THEN
						'disabled=\"disabled\"'
					ELSE
						''
					END
				
				||'
				id=\"vnid_'|| vn.vnid::text ||'\"
				
			>'
		ELSE
			to_char(vn.vndatavinculacao,'DD/MM/YYYY') 
		END as vndatavinculacao,
		to_char(vn.vndatavinculacao,'DD/MM/YYYY') as datavinculacao,
		
		CASE WHEN vnstatus = 'A' THEN
			'<input name=\'vncargahorariasemanal[]\' type=\'text\' title=\'Carga Horária - '|| 
				CASE WHEN inu.estuf IS NULL THEN	m.mundescricao || '-' || inu.mun_estuf ELSE estdescricao END
			  ||' \'
				value=\''
				|| 
					CASE WHEN vn.vncargahorariasemanal IS NOT NULL THEN vncargahorariasemanal::text ELSE '' END  
				||'\' 
				
				'||
				
					CASE WHEN vn.snid = 2 OR vn.snid = 3 THEN
						'disabled=\"disabled\"'
					ELSE
						''
					END
				
				||'
				
				
				id=\"carga_horaria_'|| vn.vnid::text ||'\"'
				
				||
				 '
			  	onkeyup=\"this.value=mascaraglobal(\'#####\',this.value);\" 
				onmouseover=\"MouseOver(this);\" 
				onfocus=\"MouseClick(this);this.select();\"
				onmouseout=\"MouseOut(this);\"
				onblur=\"MouseBlur(this);
				this.value=mascaraglobal(\'#####\',this.value);\"
			  
			  '
			  ||
				'
			   >'
		ELSE
			vn.vncargahorariasemanal::text
		END as vncargahorariasemanal,
		to_char(vn.vndatadesvinculacao,'DD/MM/YYYY') as vndatadesvinculacao,
		
			vn.dutid
		as dutid,
		vn.vnid as vnid,
		snaceito,
		CASE WHEN inu.estuf IS NULL THEN
						'Município'
					ELSE
						'Estado'
					END
						as esfera,
		vn.snid,
		
		'\'Validação\' da entidade - '
				|| 
						CASE WHEN inu.estuf IS NULL THEN	m.mundescricao || '-' || inu.mun_estuf ELSE estdescricao END
				as title_validacao,
	du.dunid,
	vn.dntvid,
	vn.vnatuacaoexclusivaei
				
	
	FROM
		par.vinculacaonutricionista  vn
	INNER JOIN par.instrumentounidade inu ON inu.inuid = vn.inuid
	LEFT JOIN par.dadosunidade  	du ON du.duncpf = vn.vncpf AND vn.inuid = du.inuid AND vn.dutid = du.dutid
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

function modificaAceite( id )
{       
		var idcheck 	=  "snaceito_"+id;
	    if( (jQuery("#"+idcheck).is(':checked')) )
	    {
	    	jQuery("#hsnaceito_"+id).val('check');
	    }
	    else
	    {
	    	jQuery("#hsnaceito_"+id).val('');
	    }   
}

function carregaTextoVinculacao( dutid, id, data, unidade)
{

	//Se for técnico
	if(!validaData(document.getElementById('data_vincula_'+id)))
	{
		alert('O campo "Data de vinculação" da entidade ' + unidade +' deve ser no formato dd/mm/YYYY.');
		jQuery('#data_vincula_'+id).val('');
		jQuery('#data_vincula_'+id).focus();
		return false;
	}
	else
	{

		var objDate = new Date();
		objDate.setYear(data.split("/")[2]);
		objDate.setMonth(data.split("/")[1]  - 1);
		objDate.setDate(data.split("/")[0]);

		if(objDate.getTime() > new Date().getTime()){
			alert("O campo 'Data de vinculação' da entidade " + unidade +"não pode ser superior a data atual.");
			jQuery('#data_vincula_'+id).val('');
			jQuery('#data_vincula_'+id).focus();
			return false;
		}
		
		
		// Se for técnico
		if(dutid == 11)
		{
			jQuery( "#snaceito_"+id).attr( "disabled", false );
			
			jQuery('#div_'+id).html(
"<div id=\"div_termo\" style=\"width: 750; min-height:50; background-color: white; padding: 7px; text-align: justify !important;\"><div align=\"center\" style=\"width: 100%;\"><b align=\"center\">Termo de Responsabilidade Técnica - "+ unidade +"</b></div><br><pre style=\"font: 12pt Arial,verdana;  text-align: justify !important; \">	Venho por meio deste informar que sou responsável técnico do Programa Nacional de Alimentação Escolar no âmbito do(a) "+ unidade +", a partir de  "+ data +", desempenhando minhas atividades em conformidade com o Código de Ética vigente (Resolução CFN nº 334/2004) Comprometo-me a cumprir e fazer cumprir o estabelecido na regulamentação do exercício profissional do Nutricionista, através de Leis Decretos ou Resoluções, bem como, assumo a responsabilidade pela veracidade das informações disponibilizadas neste formulário. </pre> </div>"

			)
		}
				
	}
}


function abreDialogValidacao( vnid )
{

	jQuery( "#dialog_validacao_"+vnid ).show();
	
	jQuery( '#dialog_validacao_'+vnid ).dialog({
			resizable: false,
			width: 630,
			modal: true,
			show: { effect: 'drop', direction: "up" },
			buttons: {
				
				'OK': function() {
					
					var radio   =  jQuery('input[name=snid_'+vnid+']:checked').val();

					if (typeof radio === 'undefined') {
					    // variable is undefined
						alert("O campo " + jQuery('#snid_'+vnid).attr('title') + " tem preenchimento obrigatório" );
						boSubmeter = false;
						return false;
					}
					else
					{
						if(radio == 1)
						{
							//jQuery('#bt_valida_'+vnid).css('background-color', 'green');
							jQuery('#bt_valida_'+vnid).val('Validado');
						}
						else if(radio == 2)
						{
							//jQuery('#bt_valida_'+vnid).css('background-color', 'red');
							jQuery('#bt_valida_'+vnid).val('Não Validado');
						}
						jQuery( this ).dialog( 'close' );
					}
				}
			}
	});
	
}


function abreDialogRespTecnico( vnid )
{

	jQuery( "#dialog_responsavel_"+vnid ).show();
	
	jQuery( '#dialog_responsavel_'+vnid ).dialog({
			resizable: false,
			width: 800,
			modal: true,
			show: { effect: 'drop', direction: "up" },
			close: function(event, ui) { 

				var radio   =  jQuery('input[name=snaceito_'+vnid+']:checked').val();
				
				if (typeof radio != 'undefined') {
					
					//jQuery('#bt_resp_tec_'+vnid).css('background-color', 'green');
					jQuery('#bt_resp_tec_'+vnid).val('Aceito');
					jQuery( this ).dialog( 'close' );
				}
				else
				{
					jQuery('#bt_resp_tec_'+vnid).val('Aceitar');
					jQuery( this ).dialog( 'close' );
		
				}

			},
			buttons: {

				'OK': function() {
					var radio   =  jQuery('input[name=snaceito_'+vnid+']:checked').val();
	
					if (typeof radio != 'undefined') {
						
						//jQuery('#bt_resp_tec_'+vnid).css('background-color', 'green');
						jQuery('#bt_resp_tec_'+vnid).val('Aceito');
						jQuery( this ).dialog( 'close' );
					}
					else
					{
						jQuery('#bt_resp_tec_'+vnid).val('Aceitar');
						jQuery( this ).dialog( 'close' );
			
					}
				}
				
			}
	});
	
}

function salvar()
{
	boSubmeter = true;
  	jQuery('form#formulario :input').each(function() 
  	{  	  	
	    if( jQuery(this).attr('type') == 'text' )
	    {
	    	var id  = jQuery(this).attr('id');
	    	if( id.indexOf("data_vincula")  == 0  )
	    	{
	    		if(!validaData(document.getElementById(id)))
	    		{
	    			alert('O campo ' + jQuery(this).attr('title') +' deve ser no formato dd/mm/YYYY.');
	    			jQuery(this).focus();
	    			boSubmeter = false;
					return false;
	    		}
	    	}
		    if( jQuery(this).val() == '')
		    {
			    
			    if( ! jQuery(this).attr("disabled"))
			    {
					alert("O campo '" + jQuery(this).attr('title') + "' tem preenchimento obrigatório" );
					jQuery(this).focus();
					boSubmeter = false;
					return false;
			    }
			}
	    }
	    else if( jQuery(this).attr('type') == 'radio' )
		{
			var nome 	= jQuery(this).attr('name');
			var radio   =  jQuery('input[name='+nome+']:checked', '#formulario').val();

			if( nome.indexOf("vnatuacaoexclusivaei_") == -1 )
			{
				if (typeof radio === 'undefined') {
				    // variable is undefined
					alert("O campo " + jQuery(this).attr('title') + " tem preenchimento obrigatório, clique no botão 'Validar' para informar a validação para o mesmo." );
					boSubmeter = false;
					return false;
				}
			}
			else
			{
				
				if( ! jQuery('input[name='+nome+']' ).attr("disabled") )
			    {
			    	var radioAt   =  jQuery('input[name='+nome+']:checked', '#formulario').val();

					if (typeof radioAt === 'undefined') {
						
						alert("O campo " + jQuery(this).attr('title') + " tem preenchimento obrigatório" );
						boSubmeter = false;
						return false;
					}
			    }
			}

		}
	    
	    else if( jQuery(this).attr('name') == "vnid[]" )
	    {
		    if(typeof jQuery("#snaceito_"+jQuery(this).val() ).attr("disabled") !== 'undefined')
		    {
		    	if( ! jQuery("#snaceito_"+jQuery(this).val() ).attr("disabled") )
			    {
		    		var nome 	= jQuery("#snaceito_"+jQuery(this).val()).attr('name');
			    	if( ! (jQuery('input[name='+nome+']').is(':checked')) )
			    	{
			    		alert("Para submeter o formulário é necessário clicar no botão 'Aceitar' e aceitar o Termo de responsabilidade do 'Responsável Técnico' da unidade " + jQuery('input[name='+nome+']').attr('title') );
			    		boSubmeter = false;
			    		return false;
			    	}
			    }
		    }
	    }
	    else if( jQuery(this).attr('type') == 'select-one' )
	    {
	    	if( ! jQuery(this).attr("disabled") )
		    {
		    	if(  jQuery(this).val() == '')
			    {
		    		var nome 	= jQuery(this).attr('name');
		    	
		    		alert("Para submeter o formulário é necessário selecionar o " + jQuery(this).attr('title') );
		    		boSubmeter = false;
		    		return false;
			    }
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


	var snid = jQuery('#snidVal').val();
	var popup = jQuery('[name="popup"]').val();
	
	if( snid == 6 || snid == 1 || popup == 'S'  )//Validou
	{
		jQuery('[name=cpf]').val(mascaraglobal('###.###.###-##',jQuery('[name=cpf]').val()));			
	}
	else// Não validou
	{
		//jQuery('#formulario_validado').hide();
		
	}

	
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


function defineValidacao(val, vnid, unidade, dunid)
{

	if( val == 6)
	{
		validacao = 'validar';
		id_campo ="#snid_"+vnid
	}
	else
	{
		validacao = 'invalidar';
		id_campo = "#snid_n_"+vnid
	}
	
	if( confirm('Tem certeza que deseja '+validacao+' o vínculo com a entidade '+unidade+'?') )
	{
		jQuery.ajax({
	        type: 'post',
	        url:  'par.php?modulo=principal/vinculacaoNutricionista&acao=A',
	        data: 'requisicao=validacaoNutricionista&snid=' + val +'&vnid='+vnid+'&dunid='+dunid ,
	        async: false,
	        success: function (res){
	
				if(res == 'sucesso')
				{
		        	if( val == 6 )//Validou
		        	{
						jQuery('#bt_valida_'+vnid).val('Validado');
						
		        		//jQuery('#formulario_validado').show();
		        		jQuery( "#data_vincula_"+vnid).attr( "disabled", false);
		        		jQuery( "#vnid_"+vnid).attr( "disabled", false);

						jQuery( "#carga_horaria_"+vnid).attr( "disabled", false );
						jQuery("#vnatuacaoexclusivaei_n_"+vnid).attr( "disabled", false );
						jQuery("#vnatuacaoexclusivaei_"+vnid).attr( "disabled", false );
						
						jQuery( "#dntvid_"+vnid).attr( "disabled", false );
	
						if(jQuery( "#data_vincula_"+vnid).val() != '' )
						{
							jQuery( "#snaceito_"+vnid).attr( "disabled", false );
						}
						
						jQuery( "#div_resp_tec_"+vnid).show();		
		        	}
		        	else// Não validou
		        	{
		        		//jQuery('#formulario_validado').hide();
		        		jQuery('#bt_valida_'+vnid).val('Não Validado');
		        		jQuery( "#data_vincula_"+vnid).attr( "disabled", true );
		        		jQuery( "#vnid_"+vnid).attr( "disabled", true );
		        		jQuery("#vnatuacaoexclusivaei_n_"+vnid).attr( "disabled", true );
						jQuery("#vnatuacaoexclusivaei_"+vnid).attr( "disabled", true );
						jQuery( "#carga_horaria_"+vnid).attr( "disabled", true );
						jQuery( "#dntvid_"+vnid).attr( "disabled", true );
						jQuery( "#snaceito_"+vnid).attr( "disabled", true );
						jQuery( "#div_resp_tec_"+vnid).hide();
		        			
	        		}

	        		alert('Salvo com sucesso');
	        		jQuery("#snid_"+vnid).attr( "disabled", true );
	        		jQuery("#snid_n_"+vnid).attr( "disabled", true );
				}
				else
				{
					alert(res);
					jQuery(id_campo).removeAttr("checked");
				}
					
			}
		});
	}
	else
	{
		
		jQuery(id_campo).removeAttr("checked");
		
	}
	
}




</script>


<?php 
	


//$situacaoOK = verificaVinculacaoNutricionista($cpfNutricionista);

$linkDadosPessoais = "par.php?modulo=principal/cadastroNutricionista&acao=A";
$linkDesvinculacao = "par.php?modulo=principal/desvinculaNutricionista&acao=A";


$arrAbas = array(
	0 => array("descricao" => "Vinculação", "link" 	=> "par.php?modulo=principal/vinculacaoNutricionista&acao=A"),
    1 => array("descricao" => "Dados pessoais", "link" 	=> "{$linkDadosPessoais}"),
	2 => array("descricao" => "Desvinculação", "link" => "{$linkDesvinculacao}")
);


echo montarAbasArray($arrAbas, "par.php?modulo=principal/vinculacaoNutricionista&acao=A");
?>
<div id="formulario_validado" <?php //todo retirar depois?>>
 <input type="hidden" id="snidVal" value="<?php echo $dadosNutri['snid'] ?>"/>
 <form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="popup" value="<?php echo $_REQUEST['popup'] ?>"/>
	<input type="hidden" name="entid" value="<?php echo $dadosNutri['entid'] ?>"/>
	<input type="hidden" name="tecnico" value="<?php echo $tecnico ?>"/>
	<input type="hidden" name="dncpf" value="<?php echo $cpfNutricionista; ?>"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<!-- 	<tr>
	       	<td align='right' class="SubTituloDireita">CPF:</td>
	        <td>
				<?php echo campo_texto('cpf', 'S', 'N', 'CPF', 20, 15, '###.###.###-##', '',null,null,null,null,null, $cpfNutricionista); ?>
				
        	</td>
      	</tr>
		<tr>
	       	<td align='right' class="SubTituloDireita">Nome:</td>
	        <td>
				<?php echo campo_texto('nome', 'S', 'N', 'Nome', 50, 50, '###.###.###-##', '',null,null,null,null,null, $dadosNutri['entnome']); ?>
				
        	</td>
      	</tr> -->

		<?php 
			
			$resultVinculacaoUnidade = $db->carregar($sqlVinculacaoUnidade);
			$resultVinculacaoUnidade = ($resultVinculacaoUnidade) ? $resultVinculacaoUnidade : Array(); 
			
			?>
			<tr >
				<td class="SubTituloDireita">Entidades vinculadas:</td>
				<td>
					<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color:333333;">
						<thead>
							<tr>
								<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
									Entidade
								</td>
								<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
									Vinculado
								</td>
								<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
									Desvinculado
								</td>
								<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
									Validação
								</td>
								<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
									Data de vinculação
								</td>
								<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
									Carga Horária Semanal
								</td>
								<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
									Data de desvinculação
								</td>
								<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
									Tipo de vínculo:
								</td>
								<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
									Atuação exclusiva na modalidade de Educação Infantil (creche e pré-escola)
								</td>
								<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
									Responsável Técnico
								</td>
								</td>
								
							</tr>
						</thead>
						<tbody>
						<?php
							foreach($resultVinculacaoUnidade as $kUni => $vUni)
							{
								
								
								$unidadeAtual = str_replace ( "'" , "\'", $vUni['unidade']);
								$enabled = '';
								$display = 'block';
								if(  ($vUni['snid'] == 2) ||  (  $vUni['snid'] == 3) ){
									
									$enabled = 'disabled=\"disabled\"';
									$display = 'none';
									
									if( $vUni['snid'] == 3 )
									{
										$valBtValidacao = 'Validar';
										$style			= '';
									}
									else 
									{
										$valBtValidacao = 'Não validado';
									}
									
								}
								elseif(  ($vUni['snid'] == 1)|| ($vUni['snid'] == 6)||  (  $vUni['snid'] == 4)||  (  $vUni['snid'] == 5) )
								{
									$valBtValidacao = 'Validado';
									//$style			= 'background-color: green;';
								}
								elseif($vUni['datavinculacao'] =='')
								{
									$enabled = 'disabled=\"disabled\"';
								}
									?>
							<tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
								<td valign="top" title="Entidade"><?=$vUni['unidade'];?></td>
								<td valign="top" title="Ativo"><?=$vUni['ativo'];?></td>
								<td valign="top" title="Inativo"><?=$vUni['inativo'];?></td>
								<td valign="top" title="Validacao">
								
									<?php if( ! ($vUni['vnstatus'] == 'I' && $vUni['snid'] == 3 ) )
										   { ?>
										<input type="button"  style="<?= $style ?>" value="<?= $valBtValidacao?>" id="bt_valida_<?=$vUni['vnid']?>" onclick="javascript:abreDialogValidacao(<?=$vUni['vnid']?>)">
									
									<div id="dialog_validacao_<?=$vUni['vnid']?>" title="Declaração de responsabilidade" style="display: none; text-align: center;">
									<div  align="center" style='width: 100%;'>
										<div  style='width: 600; 
												 		min-height:50;
												 		background-color: white;
												 		padding: 7px;
												 		text-align: justify !important;'
												 	  id="div_termo"
												 	 
											>
												<pre style="font: 12pt Arial,verdana;  text-align: justify !important; " >
	Declaro, sob as penas da lei, que as informações aqui prestadas são a expressão da verdade e que o profissional cadastrado atende ao disposto na Resolução nº 465/2010 do Conselho Federal de Nutrição.	

												</pre>
											</div>
										<?php
											$enabledRadio =  ($vUni['snid'] != '3' ) ? "disabled=disabled" : "";
											
										?>
										
										<br><input type="radio" <?= $enabledRadio ?> title="<?= $vUni['title_validacao'] ?>" value="6" name="snid_<?=$vUni['vnid']?>" id="snid_<?=$vUni['vnid']?>" <?php echo ($vUni['snid'] == '1' || $vUni['snid'] == '4' || $vUni['snid'] == '5'|| $vUni['snid'] == '6') ? "checked=checked" : ""; ?> name="snid_<?=$vUni['vnid']?>" onclick="defineValidacao(this.value, <?=$vUni['vnid']?>, '<?=$unidadeAtual?>',<?=$vUni['dunid']?> );" >Validar
										&nbsp;&nbsp;  
										<input type="radio" <?= $enabledRadio ?> title="<?= $vUni['title_validacao'] ?>" value="2" name="snid_<?=$vUni['vnid']?>" id="snid_n_<?=$vUni['vnid']?>" <?php echo ($vUni['snid'] == '2') ? "checked=checked" : ""; ?> onclick="defineValidacao(this.value, <?=$vUni['vnid']?>,'<?=$unidadeAtual?>', <?=$vUni['dunid']?>);" >Não Validar
										
									</div> 
									</div>
									<?php }?>
								</td>
								<td valign="top" title="Data de vinculação"><?=$vUni['vndatavinculacao'];?></td>
								<td valign="top" align="right" title="Carga Horária Semanal" style="color:#999999;"><?=$vUni['vncargahorariasemanal'];?><br></td>
								<td valign="top" title="Data de desvinculação"><?=$vUni['vndatadesvinculacao'];?></td>
								<td valign="top" align="right" title="Tipo de vínculo" style="color:#999999;">
									<?php if($vUni['vnstatus'] == 'A')
											{?>
									<select title="Tipo de vínculo da unidade <?=$vUni['unidade'];?>"  onchange="Tipo de vínculo(this.value)" <?=$enabled?> style="width: auto" class="CampoEstilo" name="dntvid[]" id="dntvid_<?=$vUni['vnid']?>">
										
												<option  value="">Selecione</option>
												
												<?php
		 										$sql = "SELECT dntvid as codigo, dntvdescricao as descricao FROM par.dadosnutricionistatipovinculo";
												$opcoes = $db->carregar($sql);
												
												
												foreach( $opcoes as $k => $v )
												{
													if($vUni['dntvid'] == $v['codigo'])	
													{
														$selected = 'selected="selected"';	
													}
													else
													{											
														$selected = '';
													}	
											?>
												<option <?=$selected ?> value="<?=$v['codigo']?>"><?= $v['descricao'] ?></option>
											<?php 
												}
											}?>	
									</select>
									
									
								</td>
								
								
								
 								<td valign="top" title="Data de desvinculação"> 
									<?php if($vUni['vnstatus'] == 'A')
									{?>
										<input type="radio" title="Atuação exclusiva na modalidade de Educação Infantil (creche e pré-escola) da unidade  <?=$vUni['unidade']?>" <?=$enabled?> value="t"  <?php echo ($vUni['vnatuacaoexclusivaei'] == 't') ? "checked=checked" : ""; ?> name="vnatuacaoexclusivaei_<?=$vUni['vnid']?>" id="vnatuacaoexclusivaei_<?=$vUni['vnid']?>" >Sim &nbsp; 
										<input type="radio" title="Atuação exclusiva na modalidade de Educação Infantil (creche e pré-escola) da unidade <?=$vUni['unidade']?>" <?=$enabled?> value="f"  <?php echo ($vUni['vnatuacaoexclusivaei'] == 'f') ? "checked=checked" : ""; ?> name="vnatuacaoexclusivaei_<?=$vUni['vnid']?>" id="vnatuacaoexclusivaei_n_<?=$vUni['vnid']?>">Não 
									<?php 
									}?>
								</td>
								<td valign="top" title="Responsável técnico">
								
									<?php
										
										$checked 		= ($vUni['snaceito'] == 't') ? 'checked="checked"' : '';
										$disabledck 	= ($vUni['snaceito'] == 't') ? 'disabled=disabled' : '';
										$valueHidden 	= ($vUni['snaceito'] == 't') ? 'check' : '';
									?>
									
									<input type='hidden' name="hsnaceito_<?=$vUni['vnid']?>" value="<?=$valueHidden ?>" id="hsnaceito_<?=$vUni['vnid']?>">
									
									<?php
									 if( ($vUni['snaceito'] == 't') && ($vUni['vnstatus'] == 'I') &&  ( $vUni['dutid'] == 11 )){
										echo '<center><img width="15px" border="0" src="../imagens/check_checklist.png" title="Aguardando"></center>';
									}else{	
									?>
									
										<div style="display: <?= $display ?>" id="div_resp_tec_<?= $vUni['vnid']?>">
									
										<?php if( $vUni['dutid'] == 11 )
											{
												$txtaceite = ($vUni['snaceito'] == 't') ? 'Aceito' : 'Aceitar';
											?>
											
										<input type="button" style="<?= $bkgaceite?>" value="<?=$txtaceite ?>" id="bt_resp_tec_<?=$vUni['vnid']?>" onclick="javascript:abreDialogRespTecnico(<?=$vUni['vnid']?>)">
										
										<div id="dialog_responsavel_<?=$vUni['vnid']?>" title="Responsável Técnico" style="display: none; text-align: center;">
						
										<div id="div_<?= $vUni['vnid']?>">
											<?php  // área do responsável técnico
											
											
											if($vUni['datavinculacao'] !='')
											{
												
	?>
	<div  align="center" style='width: 100%;'>
											<div  style='width: 750; 
													 		min-height:50;
													 		background-color: white;
													 		padding: 7px;
													 		text-align: justify !important;'
													 	  id="div_termo"
													 	 
												>
													

<?php 											
											
											
				echo 
"<div  align=\"center\" style='width: 100%;'><b align='center'> Termo de Responsabilidade Técnica - {$vUni['esfera']}  {$vUni['unidade']}</b></div><br>
  <pre style=\"font: 12pt Arial,verdana;  text-align: justify !important; \" >
	Venho por meio deste informar que sou responsável técnico do Programa Nacional de Alimentação Escolar no âmbito do(a) {$vUni['esfera']} {$vUni['unidade']}, a partir de {$vUni['datavinculacao']}, desempenhando minhas atividades em conformidade com o Código de Ética vigente (Resolução CFN nº 334/2004). Comprometo-me a cumprir e fazer cumprir o estabelecido na regulamentação do exercício profissional do Nutricionista, através de Leis, Decretos ou Resoluções, bem como, assumo a responsabilidade pela veracidade das informações disponibilizadas neste formulário. ";
?>

		</pre>
	</div>
</div>
	
	<?php 
											}
											else
											{
												echo  "<span style='font:8pt Arial, Verdana;'>
															Preencha o campo \"Data de vinculação\" da entidade {$vUni['unidade']} para visualizar o Termo de responsabilitade Técnica.<br>
													   </span>
														"	;
											}
											?>	
										</div>
											<?php 
												
												echo "<input type='checkbox' {$disabledck} onchange='javascript:modificaAceite({$vUni['vnid']})' {$checked} value='1' title='{$vUni['unidade']}' {$enabled} name=\"snaceito_{$vUni['vnid']}\" id=\"snaceito_{$vUni['vnid']}\"> Aceito ";
												
											?>
											</div>		
										<?php 
											
											}
											else
											{
												echo '<center>-</center>';
											}
											?>
											
										</div>	
										<?php 
										}
									?>
								</td>
							</tr>
						<?php
							}
						?>
						</tbody>
					</table>
				</td>
			</tr>

		<tr id="tr_acoes">
			<td class="SubTituloCentro" colspan="2">
	<?php 	if( $_REQUEST['popup'] == 'S' ){?>
				<input type="button" value="Fechar" id="btnfechar" onclick="window.close();">
	<?php 	} else {?>
				<input type="button" value="Prosseguir" id="btngravar" onclick="salvar()">
	 <?php	}?>
				
		</td>
		
		
	</table>
	
	
 </form>
 
 
</div>
