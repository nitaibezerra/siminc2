<?php
# Verfica se o pdeid e intid estão na sessão
pde_verificaSessao();

$entid  = $_SESSION["entid"];
$pdeid 	= $_SESSION["pdeid"];
$pdeidGet	  = $_GET['pdeid'];

if ($pdeid != $pdeidGet){
	die('<script>
			alert(\'Sistema não permite acessar duas escolas simultaneamente!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

if( $_POST['saveId'] != '' ) {	
	header('content-type: text/html; charset=ISO-8859-1'); 
		
	if( $_POST['tipo'] == 'programa'){
		$field  = "tprid";
		$field2 = "tprdescricao";
		$table  = "tipoprograma";
	}
	else {
		$field  = "tcaid";
		$field2 = "tcadescricao";
		$table  = "tipocategoria";
	}	
	$sql = "
		   UPDATE pdeescola.detalheplanoacao 
		   SET $field  = {$_POST['saveId']}
		   WHERE dpaid = {$_POST['dpaid']} 
		   "; 		   
	$sql2 = "
		   SELECT b.$field2 
		   FROM pdeescola.detalheplanoacao a
		   INNER JOIN pdeescola.$table b 
		   ON a.$field = b.$field
		   WHERE a.$field = {$_POST['saveId']}
	"; 		   
	$db->executar( $sql );
	$desc = $db->pegaUm( $sql2 );
	$db->commit();
	die( $desc );
}

if( $_POST['ajaxId'] != ''){
	
	header('content-type: text/html; charset=ISO-8859-1'); 
	
	if($_POST['tipo'] == 'programa'){
		$sql = "SELECT 
					tprid as codigo,
					tprdescricao as descricao
				FROM pdeescola.tipoprograma";
		
		$tprid = $_POST['ajaxId'];		 
        die( $db->monta_combo("tprid",$sql,'S',"Selecione...",'salvarCategoria('.$_POST["dpaid"].',this.value,\'programa\');','', '', 150, 'S', 'tprid') );
	}
	elseif( $_POST['tipo'] == 'categoria') {
		$sql = "SELECT 
					--tcaid||'_'||treid as codigo,
					tcaid as codigo,
					tcadescricao as descricao
				FROM pdeescola.tipocategoria ";

		$tcaid = $_POST['ajaxId']; 
		die( $db->monta_combo("tcaid",$sql,'S',"Selecione...",'salvarCategoria('.$_POST["dpaid"].',this.value,\'categoria\');','', '', 150, 'S', 'tcaid') );
	}
}

//$sqlPAF = " SELECT 
//				ed.entpdevlrpaf as vlrpaf
//			FROM 
//				entidade.entidadedetalhe AS ed 
//			WHERE 
//			ed.entid = '{$_SESSION['entid']}'";

$sqlPAF = " SELECT 
				coalesce(pde.pdevlrplanocapital,0) as pdevlrplanocapital,
				coalesce(pde.pdevlrplanocusteio,0) as pdevlrplanocusteio 
			FROM 
				pdeescola.pdeescola AS pde 
			WHERE 
			pde.entid = '{$_SESSION['entid']}'";


$pdevlrplano = $db->carregar( $sqlPAF );
$vlrpaf = $pdevlrplano[0]['pdevlrplanocapital']+$pdevlrplano[0]['pdevlrplanocusteio']; 
 
define( VALOR_FNDE, $vlrpaf);
 
if( $_REQUEST['excluir'] != '')
{
	$sql = "DELETE FROM pdeescola.detalheplanoacao WHERE dpaid = '{$_REQUEST['excluir']}'";
	$db->executar( $sql );
	$db->commit();
	$pseid = $_REQUEST['pseid'];
	echo("<script>alert('Operação realizada com sucesso.')\n</script>");
    echo("<script>window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/planoDeAcao&acao=A&pseid=$pseid&pdeid=".$_SESSION['pdeid']."';</script>");
	exit();		
}
	

if( $_REQUEST['pseid'] != '')
{
	
	$sql = "SELECT
				pa.plaid,
				pa.pseid,
				ps.pseidpai, 
				pa.plaliderobjetivo,
	            pa.plaindicadormeta,
	            pa.plagerenteplanoacao,
	            to_char(pa.pladatainicio::date,'MM/YYYY') as pladatainicio,
	            tr.tirdescricao,
	            to_char(pa.pladatatermino::date,'MM/YYYY') as pladatatermino,
	            ps.psedescricao
	         FROM
	         	pdeescola.planoacao AS pa
	         INNER JOIN 
	         	pdeescola.planosuporteestrategico AS ps ON ps.pseid = pa.pseid
	         INNER JOIN
	         	pdeescola.tiporevisao AS tr ON pa.tirid = tr.tirid
	         WHERE
	         	pa.pseid = '{$_REQUEST['pseid']}'";

	   
	$rsDadosMeta = $db->carregar( $sql );
	
	$pegaPai = "SELECT pseidpai FROM
	         	pdeescola.planosuporteestrategico where pseid = '{$_REQUEST['pseid']}'";
	
	if( $_REQUEST['pseid'])
	{
		$idPai = $db->pegaUm( $pegaPai );
	}
	
	
	$pegaMeta = "SELECT psedescricao FROM
	         	pdeescola.planosuporteestrategico where pseid = '{$_REQUEST['pseid']}'";
	
	if( $_REQUEST['pseid'])
	{
		$meta = $db->pegaUm( $pegaMeta );
	}
	
	$sqlEstrategia = "SELECT 
						pseidpai, 
						psedescricao 
					  FROM
					  	pdeescola.planosuporteestrategico
					  WHERE
					  	pseid = '{$idPai}'";
	if( $idPai )
	{				  	
		$rsDadosEstrategia = $db->carregar( $sqlEstrategia );
	}
		
	$sqlObjetivo = "SELECT 
						pseidpai, 
						psedescricao 
					  FROM
					  	pdeescola.planosuporteestrategico
					  WHERE
					  	pseid = '{$rsDadosEstrategia[0]['pseidpai']}'";

	if($rsDadosEstrategia[0]['pseidpai'])
	{				  	
		$rsDadosObjetivo = $db->carregar( $sqlObjetivo );
	}
	
	$plaid 				 = $rsDadosMeta[0]['plaid'];
	$pseid 				 = $rsDadosMeta[0]['pseid'];
	$plaliderobjetivo 	 = $rsDadosMeta[0]['plaliderobjetivo'];
	$plaindicadormeta 	 = $rsDadosMeta[0]['plaindicadormeta'];
	$plagerenteplanoacao = $rsDadosMeta[0]['plagerenteplanoacao'];
	$pladatainicio		 = $rsDadosMeta[0]['pladatainicio'];
	$pladatarevisao 	 = $rsDadosMeta[0]['tirdescricao'];
	$pladatatermino 	 = $rsDadosMeta[0]['pladatatermino'];
	$psedescricao 		 = $rsDadosMeta[0]['psedescricao'];
	$estrategia 		 = $rsDadosEstrategia[0]['psedescricao'];
	$objetivo 			 = $rsDadosObjetivo[0]['psedescricao'];
}

if( $plagerenteplanoacao == '')
{
	echo("<script>alert('Para visualizar esta pagina e necessário ter informado um Gerente para o Plano de Ação'); \n</script>");	
	echo("<script> history.go(-1)</script>");
	exit();
}

if( $_REQUEST['alterar'] != '')
{
	$dpaid = $_REQUEST['alterar'];
	$pseid = $_REQUEST['pseid'];
	$sql = "SELECT 
				dpaid,
				plaid,
				dpadescricaoacao,
				to_char(dpadatainicio::date,'MM/YYYY') AS dpadatainicio,
				to_char(dpadatatermino::date,'MM/YYYY') AS dpadatatermino,
				dparesponsavel,
				dparesultadoesperado,
				dpaindicador,
				forid,
				tprid,
				tcaid,
				dpavalorcapital,
				dpavalorcusteio
			FROM
				pdeescola.detalheplanoacao
			WHERE
				dpaid = '$dpaid'";
		
	$arrDados = $db->carregar( $sql );

	$plaid			      = $arrDados[0]['plaid'];
	$dpadescricaoacao  	  = $arrDados[0]['dpadescricaoacao'];
	$dpadatainicio  	  = $arrDados[0]['dpadatainicio']; 
	$dpadatatermino  	  = $arrDados[0]['dpadatatermino'];
	$dparesponsavel  	  = $arrDados[0]['dparesponsavel'];
	$dparesultadoesperado = $arrDados[0]['dparesultadoesperado'];
	$dpaindicador 		  = $arrDados[0]['dpaindicador'];
	$forid 				  = $arrDados[0]['forid'];
	$tprid 				  = $arrDados[0]['tprid'];
	$tcaid 				  = $arrDados[0]['tcaid'];
	$dpavalorca		  	  = $arrDados[0]['dpavalorcapital'];
	$dpavalorcust	 	  = $arrDados[0]['dpavalorcusteio'];
	
	$dpavalorcapital = $dpavalorca ? number_format($dpavalorca,2,',','.') : '';
	$dpavalorcusteio = $$dpavalorcust ? number_format($dpavalorcust,2,',','.') : '';

}

if( $_REQUEST['action'] == 'salvar')
{
	$perfis = arrayPerfil();
    if( ( in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis ) ) || ( in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis ) ) || ( in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis ) ) )
    {
		salvar($plaid);
    }
	else
    {
    	echo '<script type="text/javascript">alert("Você não tem permissão para inserir ou alterar dados nesta página");window.opener.location.reload(); window.close();</script>';
    	exit(); 
    }
}

function salvar($plaid)
{
	global $db;
	global $plaid;
	$pseid = $_REQUEST['pseid'];
		
	$dpadescricaoacao     = $_REQUEST['dpadescricaoacao'];
	$dpadatainicio  	  = $_REQUEST['dpadatainicio'];
	$dpadatatermino  	  = $_REQUEST['dpadatatermino'];
	$dparesponsavel  	  = $_REQUEST['dparesponsavel'];
	$dparesultadoesperado = $_REQUEST['dparesultadoesperado'];
	$dpaindicador 		  = $_REQUEST['dpaindicador'];
	$forid 				  = $_REQUEST['forid'];
	$tprid 				  = $_REQUEST['tprid_disable'];
	$arrtcaid 				  =  explode( "_", $_REQUEST['tcaid_disable']);
	$tcaid = $arrtcaid[0];
	$dpavalorcapital  	  = str_replace('.','',$_REQUEST['dpavalorcapital']);
	$dpavalorcusteio 	  = str_replace('.','',$_REQUEST['dpavalorcusteio']);

		
	$arrDatai = explode( "/", $_REQUEST['dpadatainicio']);
    $mesI = $arrDatai[0];
    $anoI = $arrDatai[1];
         
    $dataI = $anoI.'-'.$mesI.'-01';
    
    $arrDataF = explode( "/", $_REQUEST['dpadatatermino']);
    $mesF = $arrDataF[0];
    $anoF = $arrDataF[1];
           
    $dataF = $anoF.'-'.$mesF.'-01';
	
	if( $_REQUEST['alterar'] == '')
	{
	   if(trim($_REQUEST['forid']) != '')
	   {
		  if($_REQUEST['dpadatainicio'])
		  {
			if(($_REQUEST['dpadatatermino']))
			{
			$sql = "INSERT INTO 
					pdeescola.detalheplanoacao
					(   plaid,
						dpadescricaoacao,
						dpadatainicio,
						dpadatatermino,
						dparesponsavel,
						dparesultadoesperado,
						dpaindicador,
						forid,
						tprid,
						tcaid,
						dpavalorcapital,
						dpavalorcusteio	
					)
					VALUES
					(   '$plaid',
						'".substr($_REQUEST['dpadescricaoacao'], 0, 2999 )."',
						'".str_replace( '%','',str_replace( ' ', '',$dataI ) )."',
						'".str_replace( '%','',str_replace( ' ', '',$dataF ) )."',
						'$dparesponsavel',
						'".substr($_REQUEST['dparesultadoesperado'], 0, 500 )."',
						'".substr($_REQUEST['dpaindicador'], 0, 500 )."',
						 ".($forid ? $forid : 0 ).", 
						 ".($tprid ? $tprid : 'NULL').",
						 ".($tcaid ? $tcaid : 'NULL').",";
						
						if($dpavalorcapital == '')
						{
							$sql.="".($dpavalorcapital ? str_replace( ',','.',str_replace(' ', '',$dpavalorcapital ) )  : 'NULL').","; 
						}
						else
						{
							$sql.="".($dpavalorcapital ? str_replace( ',','.',str_replace(' ', '',$dpavalorcapital) ) : 'NULL').","; 
						}
						
						if($dpavalorcusteio == '')
						{
							$sql.="".($dpavalorcusteio ? str_replace( ',','.',str_replace(' ', '',$dpavalorcusteio) ) : 'NULL').""; 
						}
						else
						{
							$sql.="".($dpavalorcusteio ? str_replace( ',','.',str_replace(' ', '',$dpavalorcusteio) ) : 'NULL').""; 
						}
						$sql.=')';  
						
			$insert = $db->executar( $sql );
			$db->commit();	
			echo("<script>alert('Operação realizada com sucesso.')\n</script>");
		    echo("<script>window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/planoDeAcao&acao=A&pseid=$pseid&pdeid=".$_SESSION['pdeid']."'</script>");
			exit();
			}
			else
			{
				echo("<script>alert('Digite uma data válida para campo: Data Término');\n</script>");
				echo("<script> history.go(-1)</script>");
				exit();
			}
		 }
		 else
		 {
			echo("<script>alert('Digite uma data válida para campo: Data Início')\n</script>");
			echo("<script> history.go(-1)</script>");
			exit();
		 }
	  }
	  else
	  {
		 echo("<script>alert('Você deve selecionar uma Fonte de Recursos')\n</script>");
		 echo("<script> history.go(-1)</script>");
		 exit();
	  }
	}
	else
	{
	  if($_REQUEST['forid'])
	  {
      	if($_REQUEST['dpadatainicio'])
	  	{
	  		if(($_REQUEST['dpadatatermino']))
			{
			$sql = "UPDATE
						pdeescola.detalheplanoacao
					SET
						dpadescricaoacao = '".substr($dpadescricaoacao, 0, 2999 )."',
						dpadatainicio = '".str_replace( '%','',str_replace( ' ','',$dataI ))."',
						dpadatatermino = '".str_replace( '%','',str_replace( ' ','',$dataF ))."',
						dparesponsavel = '$dparesponsavel',
						dparesultadoesperado = '".substr($dparesultadoesperado, 0, 500 )."',
						dpaindicador = '".substr($dpaindicador, 0, 500 )."',
						forid =  ".($forid ? $forid : 0 )." ,
						tprid =  ".($tprid ? $tprid : 'NULL')." ,
						tcaid =  ".($tcaid ? $tcaid : 'NULL')." ,";
						
						if($dpavalorcapital == '')
						{
							$sql.=" dpavalorcapital = ".($dpavalorcapital ? str_replace( ',','.',str_replace(' ','',$dpavalorcapital) ) : "NULL").","; 
						}
						else
						{
							$sql.=" dpavalorcapital = ".($dpavalorcapital ? str_replace( ',','.',str_replace(' ','',$dpavalorcapital) ) : "NULL").","; 
						
						}
						if($dpavalorcusteio == '')
						{
							$sql.=" dpavalorcusteio = ".($dpavalorcusteio ? str_replace( ',','.',str_replace(' ','',$dpavalorcusteio) ) : "NULL").""; 
						}
						else
						{
							$sql.=" dpavalorcusteio = ".($dpavalorcusteio ? str_replace( ',','.',str_replace( ' ','',$dpavalorcusteio) ) : "NULL").""; 
						}
						
						/*
						dpavalorcapital = '".str_replace( ',', '',$dpavalorcapital)."',
						dpavalorcusteio =  '".str_replace( ',', '',$dpavalorcusteio)."'
						*/	
			$sql .= "
				 	WHERE
						dpaid = '{$_REQUEST['alterar']}'";

			$update = $db->executar( $sql );
			$db->commit();
			echo("<script>alert('Operação realizada com sucesso.')\n</script>");
echo("<script>window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/planoDeAcao&acao=A&pseid=$pseid&pdeid=".$_SESSION['pdeid']."'</script>");
			exit();
			}
			else
			{
				echo("<script>alert('Digite uma data válida para campo: Data Término');\n</script>");
				echo("<script> history.go(-1)</script>");
				exit();
			}
		 }
		 else
		 {
			echo("<script>alert('Digite uma data válida para campo: Data Início')\n</script>");
			echo("<script> history.go(-1)</script>");
			exit();
		 }
	  }
	  else
	  {
		 echo("<script>alert('Você deve selecionar uma Fonte de Recursos')\n</script>");
		 echo("<script> history.go(-1)</script>");
		 exit();
	  }
	}

}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pdeid 	= $_SESSION["pdeid"];
$entid  = $_SESSION["entid"];
if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}


//busca os valores ja utilizados do custeio e capital da escola

$sqlvalor = "SELECT 
				p.pseid,
				d.dpavalorcusteio,
				d.dpavalorcapital
			FROM 
				pdeescola.planoacao AS p
			INNER JOIN 
				pdeescola.detalheplanoacao as d ON d.plaid = p.plaid
			INNER JOIN 
				pdeescola.planosuporteestrategico AS s ON p.pseid = s.pseid
			WHERE 
				s.pdeid = '{$_SESSION['pdeid']}'
			AND
				d.forid = 5";
 
$rsValor = $db->carregar( $sqlvalor );
for( $v = 0; $v < count( $rsValor); $v++)
{
	$total_custeio += $rsValor[$v]['dpavalorcusteio'];
	$total_capital += $rsValor[$v]['dpavalorcapital'];
}


$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Plano de Ação ', '');
echo cabecalhoPDE(); 

?>

<?php
 
/*----------------- Recupera dados ---------------------*/  
$dados = $db->pegaLinha("SELECT pdeuf,
								pdemunicipio, 
								pdenome,
								pdenomediretor,
								pdebairro,
								pdelogradouro,
								pdecomplemento,
								pdetelefone,
								pdeemail,
								tloid
						FROM pdeescola.pdeescola
						WHERE pdeid = '".$pdeid."'");                                                   
extract($dados);
if($dados['tloid'] == 1){
	$check1 = 'checked="checked"';
}else if($dados['tloid'] == 2){
	$check2 = 'checked="checked"';
}else if($dados['tloid'] == 3){
	$check3 = 'checked="checked"';
}

$sql = "SELECT entnome as nome
		FROM entidade.entidade as e
		INNER JOIN entidade.funcaoentidade as fe ON fe.entid = e.entid
		WHERE funid = 3 and
		  	  tpcid IN (1,3) AND
	    	  e.entid IN ('{$entid}')";
$NomeEscola = $db->pegaum($sql);
$pdenome = $NomeEscola;

?>


<?=subCabecalho(' Desdobramento das Metas em Planos de Ação ');?>

<?php
#aqui o formulario
?>

   <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

	 <style type="text/css" media="screen">
        #div_capital
        {
            background-color: #ffffff;
            /*color: #000033;*/
            color: red;
            width: 110px;
            /*border: 2px solid #cccccc;*/
            font-size: 12px; 
            font-weight: bold;
            visibility: hidden;
        }

        #div_custeio {
            background-color: #ffffff;
             color: red;
            width: 110px; 
             
            font-size: 12px; 
            font-weight: bold;
            visibility: hidden;
        }
    </style>
<form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario" >

	<input type="hidden" name="CapitalObrigatorio" id="CapitalObrigatorio">
	<input type="hidden" name="CusteioObrigatorio" id="CusteioObrigatorio">

    <table  align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
		<tr>
			<td class="SubTituloDireita">
				Descrição:
				<input type="hidden" name="action" value=0 id = "action">
			</td>
			<td  colspan="2">
				<?= campo_textarea( 'dpadescricaoacao', 'S', 'S', 'Descrição ', 100 , 12, 3000, $funcao = '', $acao = 0, $txtdica = '', $tab = false ); ?>             
	    	</td>
		</tr>
		
		<tr>
			<td class="subtitulodireita">
				Data de Início:
			</td> 
			<td align="left"  colspan="2">
				 	
                <?= campo_texto('dpadatainicio', 'S', $somenteLeitura, '', 20, 100, '##/####', '', 'left', '',  0, 'id="dpadatainicio"', '', '', 'validarMesAno( this )' ); ?>
                
            </td>
      	</tr>	
		<tr>
			<td class="subtitulodireita">
				Data de Término:
			</td> 
			<td align="left"  colspan="2">

                <?= campo_texto('dpadatatermino', 'S', $somenteLeitura, '', 20, 100, '##/####', '', 'left', '',  0, 'id="dpadatatermino"', '', '', 'validarMesAno( this )' ); ?>
                
            </td>
      	</tr>	
		<tr>
			<td class="SubTituloDireita" >
				Responsável:
			</td>
			<td colspan="2">
				<?= campo_texto('dparesponsavel', 'S', $somenteLeitura, '', 97, 100, '', '', 'left', '',  0, 'id="dparesponsavel" onblur="MouseBlur(this);"' ); ?>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
				Resultado Esperado:
			</td>
			<td colspan="2">
				<?= campo_textarea( 'dparesultadoesperado', 'S', 'S', 'Descrição ', 100 , 12, 500, $funcao = '', $acao = 0, $txtdica = '', $tab = false ); ?>             
	    	</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
				Indicador:
			</td>
			<td colspan="2">
				<?= campo_textarea( 'dpaindicador', 'S', 'S', 'Descrição ', 100 , 12, 500, $funcao = '', $acao = 0, $txtdica = '', $tab = false ); ?>             
	    	</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
				Fonte de Recursos:
			</td>
			<td colspan="2">
			<?php
			$sql = "SELECT 
						forid as codigo,
						fordescricao as descricao
					FROM pdeescola.fonterecurso";
			
        	$db->monta_combo("forid",$sql,'S',"Selecione a Fonte de Recursos",'verificaFonteRecurso(value);','', '', 300, 'S', 'forid');
        	
			?>	
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
				Tipo do Programa:
			</td>
			<td colspan="2">
			<?php
			$sql = "SELECT 
						tprid as codigo,
						tprdescricao as descricao
					FROM pdeescola.tipoprograma";
			
        	$db->monta_combo("tprid",$sql,'N',"Selecione o Tipo de Programa",'','', '', 300, 'S', 'tprid');
			?>	
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Categoria:
			</td>
			<td  colspan="2" align="left">
			<?php
			$sql = "SELECT 
						--tcaid||'_'||treid as codigo,
						tcaid as codigo,
						tcadescricao as descricao
					FROM pdeescola.tipocategoria ";
			$db->monta_combo("tcaid",$sql,'N',"Selecione a Categoria",'','', '', 300, 'S', 'tcaid');
			
			?>	
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
				Valor Capital:
			</td>
			<td colspan="2"> 
				<?= campo_texto('dpavalorcapital', 'S', $somenteLeitura, '', 20, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="dpavalorcapital" onblur="MouseBlur(this); desbloqueiaCampo(name); "', 'calculaCapital(this.value); bloqueiaCampo(name);','', "this.value = mascaraglobal('#.###.###.###,##',  document.getElementById('dpavalorcapital').value )" ); ?>
           	<?php
           		//$resta_percent_capital = (VALOR_FNDE * 30)/100;
           		$resta_percent_capital = $pdevlrplano[0]['pdevlrplanocapital']; 
				$capital_restante = ($resta_percent_capital - $total_capital);
				echo("<div id =\"div_capital\">"); 
				echo number_format($capital_restante,2,',','.');
				echo("</div>");
				?>	
				<input type="hidden" name="capital" id="capital" value="<?=$capital_restante?>">
	           	<input type="hidden" name="total_permitido_capital" id="total_permitido_capital" value="<?=$resta_percent_capital?>">
	           
           	</td>
           	<td>
           	
            </td>
		</tr>		 
		<tr>
			<td class="SubTituloDireita" >
				Valor Custeio:
		 	</td>
		 	<td colspan="2">
				<?= campo_texto('dpavalorcusteio', 'S', $somenteLeitura, '', 20, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="dpavalorcusteio" onblur="MouseBlur(this); desbloqueiaCampo(name); " ', 'calculaCusteio(this.value); bloqueiaCampo(name);', '', "this.value = mascaraglobal('#.###.###.###,##',  document.getElementById('dpavalorcusteio').value )" ); ?>
           <?php
//           		$resta_percent_custeio = (VALOR_FNDE * 70)/100;
				$resta_percent_custeio = $pdevlrplano[0]['pdevlrplanocusteio'];
				$custeio_restante = ($resta_percent_custeio - $total_custeio); 
				echo("<div id =\"div_custeio\">");
				echo number_format($custeio_restante,2,',','.');
				echo("</div>");
				?>	
				<input type="hidden" name="custeio" id="custeio" value="<?=$custeio_restante?>">
				<input type="hidden" name="total_permitido_custeio" id="total_permitido_custeio" value="<?=$resta_percent_custeio?>">
           
           	</td>
            <td>
             	
            </td>
		</tr>	
		
		<tr>
			<td class="SubTituloDireita" colspan="2">
				
			</td>
			<td>
				<input type="button" value="Salvar" name="btnSalvar" onClick="return validaForm();">
				<input type="button" value="Voltar" name="btnVoltar" onclick="javascript: history.go(-2);"/>	
			</td>
		</tr>		 
  
	</table>
</form>

        <?php
        	$sql = "
        			SELECT  
        			    d.dpaid,
				       '<img onclick=\"return alterarDetalhe('|| d.dpaid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirDetalhe('|| d.dpaid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />' as acoes,
						d.dpadescricaoacao,
				        to_char(dpadatainicio::date,'MM/YYYY') as dataini,
				        to_char(dpadatatermino::date,'MM/YYYY') as datafim,
                       	dparesponsavel,
                       	
                       	--substr(d.dparesultadoesperado, 1, 100)||'...' as resultado,
                        dparesultadoesperado,
						
                       	--substr(d.dpaindicador, 1, 100)||'...' as indicador,
                       	dpaindicador,

                       	
						d.dpavalorcapital,
						d.dpavalorcusteio,
						
                       	f.fordescricao,
                       	p.tprdescricao,
                       	p.tprid,
                       	t.tcaid, 
                       	t.tcadescricao
                       	
					FROM
						pdeescola.detalheplanoacao as d
					INNER JOIN 
						pdeescola.fonterecurso as f on f.forid = d.forid
					LEFT JOIN 
						pdeescola.tipocategoria as t on t.tcaid = d.tcaid
					LEFT JOIN 
						pdeescola.tipoprograma as p on p.tprid = d.tprid
					WHERE
						plaid = '$plaid'
					ORDER BY d.dpaid asc						
        			";
			if($plaid != '' ){
				$dados = $db->carregar( $sql );
			}

	
			 
        	//$cabecalho = array("Ações", "Descrição","Início", "Término", "Responsável", "Resultado Esperado", "Indicador", "Capital (R$)", "Custeio (R$)", "Quem Financia", "Tipo do Programa", "Categoria" );
        	
         	//$db->monta_lista_simples($sql, $cabecalho, 20, 10, 'N', '', '', true );
     	?>

<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
<thead>
<tr>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Nº
	</td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Ações
	</td> 
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Descrição
	</td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Início
	</td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Término
	</td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Responsável
	</td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Resultado Esperado
	</td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Indicador
	</td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Capital (R$)
	</td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Custeio (R$)
	</td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Quem Financia
	</td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Tipo do Programa
	</td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
		Categoria
	</td>
</tr> 
</thead>
<tbody>

<?php

 

for( $i = 0; $i < count( $dados ); $i++ )
{
	
	if( $dados )
	{
	?>
	<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
		<td title="Descrição">
			<b><?= $i+1 ?></b>
		</td>
		<td title="Ações">
			<img onclick="return alterarDetalhe(<? echo $dados[$i]['dpaid']; ?>)" src="/imagens/alterar.gif" border="0" title="Alterar Registro" /><img onclick="return excluirDetalhe(<?=$dados[$i]['dpaid']?>)" src="/imagens/excluir.gif" border="0" title="Excluir Registro" />
		</td>
		<td title="Descrição">
			<?=$dados[$i]['dpadescricaoacao']?>
		</td>
		<td title="Início">
			<?=$dados[$i]['dataini']?>
		</td>
		<td title="Término">
			<?=$dados[$i]['datafim']?>
		</td>
		<td title="Responsável">
			<?=$dados[$i]['dparesponsavel']?>
		</td>
		<td title="Resultado Esperado">
			<?=$dados[$i]['dparesultadoesperado']?>
		</td>
		<td title="Indicador">
			<?=$dados[$i]['dpaindicador']?>
		</td>
		<td title="Capital (R$)" style="color:#3399FF;" title="Custeio (R$)">
			<?=$dados[$i]['dpavalorcapital']?>
		</td>
		<td align="right" style="color:#3399FF;" title="Custeio (R$)">
			<?=$dados[$i]['dpavalorcusteio']?>
		</td>
		<td title="Quem Financia">
			<?=$dados[$i]['fordescricao']?>
		</td>
		<td title="Tipo do Programa">
					
			<?php
			echo"<div id='programa[{$dados[$i]['dpaid']}]'>";
			echo $dados[$i]['tprdescricao'];
			$perfis = arrayPerfil(); 
		    if( ( in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfis ) ) || ( in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis ) ) ){	 
				if( $dados[$i]['tprid'] != '' ) {
					echo"<br><a style='color: blue; text-decoration: underline; cursor: pointer;' onclick='alterar( {$dados[$i]['tprid']}, \"programa\", {$dados[$i]['dpaid']});'> alterar</a>";
				}
		    }
			echo"</div>";		
			?>	
		</td>
		<td title="Categoria"> 
			<?php
			echo"<div id='categoria[{$dados[$i]['dpaid']}]'>";
			echo $dados[$i]['tcadescricao'];
			
			$perfis = arrayPerfil(); 
		    if( ( in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfis ) ) || ( in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis ) ) ){
				if( $dados[$i]['tcaid'] != '' ) {
					echo"<br><a style='color: blue; text-decoration: underline; cursor: pointer;' onclick='alterar( {$dados[$i]['tcaid']}, \"categoria\", {$dados[$i]['dpaid']});'> alterar</a>";
				}
		    }
			echo"</div>";	
			?>	
		</td>
	</tr>
<?php
	}
}
?>
</tbody>
</table>

<script>



function salvar() 
{
	 /*
	 var div_custeio = document.getElementById('div_custeio').innerHTML;
	 val_custeio = div_custeio;
	 val_custeio = val_custeio.toString().replace(".","");
	 val_custeio = val_custeio.toString().replace(",",".");
	 
	 if( val_custeio < 0)
	 {
	 	alert('Você não pode exceder o valor disponível para a escola.');
	 	return false;
	 } 
	 */
	 document.formulario.action.value="salvar";
	 document.formulario.submit();
	 return true;
}

function valida_Data()
{
	var datai = document.getElementById( "dpadatainicio" ).value;
    var arrdatai = datai.split( "/" );
    
    var mesi = parseFloat(arrdatai[0]);
    var anoi = parseFloat(arrdatai[1]);
  
  	var dti = document.getElementById('dpadatainicio').value.length;
 	
  	if( dti != 7 )
  	{
  		alert('Digite uma data no formato: mm/aaaa');
  		return false;
  	}
  	
    if(( mesi == 0) || (mesi > 12 ))
    {
        alert('Data inexistente para Data de Início');
        return false;
    }
    
    if(anoi < 2008) 
    {
        alert('Ano não permitido para Data de Início.');
        return false;
    }
    
    var dataf = document.getElementById( "dpadatatermino" ).value;
    var arrdataf = dataf.split( "/" );
    
    var mesf = parseFloat(arrdataf[0]);	
    var anof = parseFloat(arrdataf[1]);
    
    var dtf = document.getElementById('dpadatatermino').value.length;
    
  	if( dtf != 7 )
  	{
  		alert('Digite uma data no formato: mm/aaaa');
  		return false;
  	}

    if(( mesf == 0) || (mesf > 12 )) 
    {
        alert('Data inexistente para Data de Término');
        return false;
    }
    
    if(anof < 2008) 
    {
        alert('Ano não permitido para Data de Término.');
        return false;
    }
    
    if(anoi > anof) 
    {
        alert('A data inicial não pode ser maior do que a data final.');
        return false;
    }
    else if (anoi == anof) 
    {
        if(mesi > mesf) 
        {
            alert('A data inicial não pode ser maior do que a data final.');
            return false;
        }
    }
    //tratando também dentro do valida_Data
    if( !validarMesAno( document.getElementById( 'dpadatainicio'  ) ) ) return false;
    if( !validarMesAno( document.getElementById( 'dpadatatermino' ) ) ) return false;
    
    return true;
}

function calculaCusteio( valor )
{
	if( top.obrigatorio == 'sim' )
	{
		
		var total_custeio = document.getElementById('custeio').value;

		<?if( ( $_REQUEST['alterar'] != '') && ( $forid == 5 ) ){?>
			var vlAlterarcust = "<?=$dpavalorcust?>";
			if(!vlAlterarcust) vlAlterarcust = 0;
			total_custeio = parseFloat(total_custeio) + parseFloat(vlAlterarcust);
		<?}?>

		var valor = valor.replace(".","");
		var valor = valor.replace(".","");
		var valor = valor.replace(".","");
		var valor = valor.replace(".","");
		var valor = valor.replace(",",".");
		
		var restante_custeio = Number(total_custeio - valor);
		var div_custeio = document.getElementById('div_custeio');
	 	if(restante_custeio < 0)
		{
			alert('Você não pode exceder o valor disponível para a escola.');
	 		div_custeio.innerHTML = float2moeda(total_custeio);
	 		document.getElementById('dpavalorcusteio').value = '';
	 	}
	 	else
	 	{
	 		div_custeio.innerHTML = float2moeda(restante_custeio);
	 	}
	 } 	 	
}

function float2moeda(num) {
   x = 0;
   if(num<0) {
      num = Math.abs(num);
      x = 1;
   }
   if(isNaN(num)) num = "0";
      cents = Math.floor((num*100+0.5)%100);
   num = Math.floor((num*100+0.5)/100).toString();
   if(cents < 10) cents = "0" + cents;
      for (var i = 0; i < Math.floor((num.length-(1+i))/3); i++)
         num = num.substring(0,num.length-(4*i+3))+'.'
               +num.substring(num.length-(4*i+3));
    ret = num + ',' + cents;
    if (x == 1) ret = ' - ' + ret;
    return ret;
}

function calculaCapital( valor )
{
	if( top.obrigatorio == 'sim' )
	{
		var total_capital = document.getElementById('capital').value;

		<?if( ( $_REQUEST['alterar'] != '') && ( $forid == 5 ) ){?>
			var vlAlterarcap = "<?=$dpavalorca?>";
			if(!vlAlterarcap) vlAlterarcap = 0;
			total_capital = parseFloat(total_capital) + parseFloat(vlAlterarcap);
		<?}?>

		var valor = valor.replace(".","");
		var valor = valor.replace(".","");
		var valor = valor.replace(".","");
		var valor = valor.replace(",",".");
		
		var restante_capital = Number(total_capital - valor);
		var div_capital = document.getElementById('div_capital');
			if(restante_capital < 0)
			{
				alert('Você não pode exceder o valor disponível para a escola.');
				div_capital.innerHTML = float2moeda(total_capital);
		 		document.getElementById('dpavalorcapital').value = '';
		 	}
		 	else
		 	{
		 		div_capital.innerHTML = float2moeda(restante_capital);
		 	} 	
 	}
}


function validaForm() 
{
	
	if( valida_Data() )
	{
		if (!validaBranco(document.formulario.dpadescricaoacao, 'Descrição')) return;
		if (!validaBranco(document.formulario.dpadatainicio,'Data de Início')) return;
		if (!validaBranco(document.formulario.dpadatatermino, 'Data de Término')) return;
		if (!validaBranco(document.formulario.dparesponsavel, 'Responsável')) return;
		if (!validaBranco(document.formulario.dparesultadoesperado, 'Resultado Esperado')) return;
		if (!validaBranco(document.formulario.dpaindicador,'Indicador')) return;

		
		if( document.formulario.forid.value == '' )
		{
			alert('Você deve selecionar uma Fonte de Recursos');
			return false;
		}
		

		var forid = document.getElementById('forid');
		
		if(forid.value == 5)
		{
		
			if( document.getElementById('tprid').value == '' )
			{
				alert('Você deve selecionar um Tipo de Programa');
				return false;
			}
			if( document.getElementById('tcaid').value == '' )
			{
				alert('Você deve selecionar uma Categoria');
				return false;
			}
			if(document.formulario.dpavalorcapital.value == '' && document.formulario.dpavalorcusteio.value == ''){
				alert('Você deve informar o Valor Capital ou o Valor Custeio');
				return false;
			}
			if(document.formulario.dpavalorcapital.value != ''){
				var v1 = document.formulario.dpavalorcapital.value;
				var v1 = v1.replace(".","");
				var v1 = v1.replace(".","");
				var v1 = v1.replace(".","");
				var v1 = v1.replace(",",".");

				if(parseFloat(v1) <= 0){
					alert('O Valor Capital deve ser maior que zero!');
					return false;
				}
			}
			if(document.formulario.dpavalorcusteio.value != ''){
				var v1 = document.formulario.dpavalorcusteio.value;
				var v1 = v1.replace(".","");
				var v1 = v1.replace(".","");
				var v1 = v1.replace(".","");
				var v1 = v1.replace(",",".");

				if(parseFloat(v1) <= 0){
					alert('O Valor Custeio deve ser maior que zero!');
					return false;
				}
			}
			
		}
		else{

			if( document.formulario.CapitalObrigatorio.value == 'verdadeiro')
			{
				if (!validaBranco(document.formulario.dpavalorcapital, 'Valor Capital')) return;
			}
			
			document.formulario.dpavalorcapital.value = mascaraglobal('#.###.###.###,##', document.formulario.dpavalorcapital.value);
				
			if( document.formulario.CusteioObrigatorio.value == 'verdadeiro')
			{
				if (!validaBranco(document.formulario.dpavalorcusteio, 'Valor Custeio')) return;
			}	
				
			document.formulario.dpavalorcusteio.value = mascaraglobal('#.###.###.###,##', document.formulario.dpavalorcusteio.value);			
		}
		
		return salvar();
	} 
	
	 
}

function verificaFonteRecurso(id)
{ 
	var id;
	
	if( id == 5 )
	{
		document.getElementById('div_capital').style.visibility= "visible";
		document.getElementById('div_custeio').style.visibility= "visible";
		document.getElementById('tprid').disabled = 0;
		document.getElementById('tcaid').disabled = 0;
		top.obrigatorio = 'sim';
	}
	else
	{
		document.getElementById('div_capital').style.visibility= "hidden";
		document.getElementById('div_custeio').style.visibility= "hidden";
		document.getElementById('tprid').disabled = 1;
		document.getElementById('tcaid').disabled = 1;
		top.obrigatorio = 'nao';
	}

	<?if($_REQUEST['alterar']){?>
		
		var vlAlterarFontecap = "<?=$dpavalorca?>";
		var vlAlterarFontecust = "<?=$dpavalorcust?>";

		if(vlAlterarFontecap){
			document.getElementById('dpavalorcapital').value = float2moeda(vlAlterarFontecap);
			calculaCapital( float2moeda(vlAlterarFontecap) );
		}
		else{
			document.getElementById('dpavalorcapital').value = '';
		}

		if(vlAlterarFontecust){
			document.getElementById('dpavalorcusteio').value = float2moeda(vlAlterarFontecust);
			calculaCusteio( float2moeda(vlAlterarFontecust) );
		}
		else{
			document.getElementById('dpavalorcusteio').value = '';
		}
		
	<?}else{?>
		document.getElementById('dpavalorcapital').value = '';
		document.getElementById('dpavalorcusteio').value = '';
	<?}?>


}



function verificaCategoria(id)
{ 
	var id;
	if( id == '11_2' )
	{  

		 var div_custeio = document.getElementById('custeio').value; 
		 val_custeio = div_custeio;
		 //val_custeio = val_custeio.toString().replace(".","");
		 // val_custeio = val_custeio.toString().replace(",",".");
		 
		 var total_custeio = Number(val_custeio * 15)/100;
		 document.getElementById('div_custeio').innerHTML = float2moeda(total_custeio);	
		 top.categoria = 'true';
		 top.tot_custeio = total_custeio;
	 
	}
	else
	{ 
		 var div_custeio = document.getElementById('custeio').value;
		 val_custeio = div_custeio;
		 
		 //var total_custeio = Number(val_custeio * 70)/100;
		 document.getElementById('div_custeio').innerHTML = float2moeda(val_custeio);	
		 top.categoria = 'false';
	
	}
	var treid = id.split("_");
	
	if( treid[1] == '1' )
	{	
		
		document.getElementById('dpavalorcapital').disabled = 0;
		document.getElementById('dpavalorcusteio').disabled = 1;
		document.formulario.CapitalObrigatorio.value = 'verdadeiro';
	}
	else if( treid[1] == '2')
	{
		
		document.getElementById('dpavalorcusteio').disabled = 0;
		document.getElementById('dpavalorcapital').disabled = 1;
		document.formulario.CusteioObrigatorio.value = 'verdadeiro';
	}
}


function bloqueiaCampo( name )
{
	var name;
	if( name == 'dpavalorcapital' )
	{
		var tam1 = document.formulario.dpavalorcapital.value.length;
		if( Number(tam1) > 0 )
		{ 
			document.getElementById('dpavalorcusteio').value = '';
			document.getElementById('dpavalorcusteio').disabled = 1;
			document.formulario.CapitalObrigatorio.value = 'verdadeiro';
		}
		else
		{
			document.getElementById('dpavalorcusteio').disabled = 0;			
//			document.getElementById('dpavalorcapital').disabled = 0;
		}
	}
	else
	{
		if( name == 'dpavalorcusteio')
		{
			var tam2 = document.formulario.dpavalorcusteio.value.length;
			
			if( tam2 > 0 )
			{
				document.getElementById('dpavalorcapital').value = '';
				document.getElementById('dpavalorcapital').disabled = 1;
				document.formulario.CusteioObrigatorio.value = 'verdadeiro';
			}
			else
			{
				document.getElementById('dpavalorcapital').disabled = 0;
//				document.getElementById('dpavalorcusteio').disabled = 0;
			}
		}
	}
}

function desbloqueiaCampo(name)
{
	var name;
	
	if( name == 'dpavalorcapital')
	{
		var tam = document.formulario.dpavalorcapital.value.length;
		if( Number( tam ) < 1)
		{
			document.getElementById('dpavalorcusteio').disabled = 0;
			document.formulario.CapitalObrigatorio.value = 'false';
		}
	
	}
	else
	{
		if( name == 'dpavalorcusteio')
		{
			var tam = document.formulario.dpavalorcusteio.value.length;
			if( Number( tam ) < 1)
			{
				document.getElementById('dpavalorcapital').disabled = 0;
				document.formulario.CusteioObrigatorio.value = 'false';
			}
		}
	}	
		
}


function alterarDetalhe( dpaid )
{
	if((dpaid != '') &&  (dpaid != null))
		return window.location.href = 'pdeescola.php?'+"<?php echo $_SERVER['argv'][0]; ?>"+'&alterar='+dpaid;
}

function excluirDetalhe( dpaid )
{
	if((dpaid != '')&&  (dpaid != null))
	{
	    if (!confirm('Deseja excluir o registro?')) 
	    {
	    	return false;
		}
		return window.location.href = 'pdeescola.php?'+"<?php echo $_SERVER['argv'][0]; ?>"+'&excluir='+dpaid;
	}
}
/*
//ocultar campos qdo selecao campo Fonte de Recursos = PAF
function exibirlinhas(){

	var forid = document.getElementById('forid');
	
	if(forid.value == 5){
		document.getElementById('linhaTipoPrograma').style.display = "block";
		document.getElementById('linhaCategoria').style.display = "block";
	}else{
		document.getElementById('linhaTipoPrograma').style.display = "none";
		document.getElementById('linhaCategoria').style.display = "none";
	}
}
*/
/* 
function ajaxCarregaCategoria( forid ){
	if ( forid ) {
		var url = caminho_atual + '?'+"<?php echo $_SERVER['argv'][0]; ?>"+'&subAcao=ajaxCarregaCategoria&AJAX=1&forid=' + forid;
			
		var myAjax = new Ajax.Updater(
			"spanComboCategoria",
			url,
			{
				method: 'get',
				asynchronous: false
			});
	}

}    
*/    

//verificando data no onblur
function validarMesAno( obj ){

	obj.value = mascaraglobal( '##/####', obj.value ); 

	if( obj.value ) {
	
		var valorInicial = obj.value;
		
		obj.value = '01/'+obj.value;
		
		if( !validaData( obj ) ){
			obj.value = "";
			alert( "Data Inválida" );
			return false;
		}
		obj.value = valorInicial;
		return true;
	}
	
}


function alterar( id, tipo, dpaid )
{ 
	var div = document.getElementById(tipo+'['+dpaid+']');
 
	var req = new Ajax.Request('pdeescola.php?modulo=principal/sintese_autoavaliacao/planoDeAcao&acao=A', {
							        method:     'post',
							        parameters: '&ajaxId=' + id +'&tipo='+tipo +'&dpaid='+dpaid,							         
							        onComplete: function (res)
							        {	
							        	 div.innerHTML = res.responseText; 
							        }
							        
							  });	
	
} 

function salvarCategoria( dpaid, id, tipo)
{
	if( id == '' ){
		return false;
	} 
 	var div = document.getElementById(tipo+'['+dpaid+']');
 
	var req = new Ajax.Request('pdeescola.php?modulo=principal/sintese_autoavaliacao/planoDeAcao&acao=A', {
							        method:     'post',
							        parameters: '&saveId='+id +'&dpaid='+dpaid +'&tipo='+tipo,							         
							        onComplete: function (res)
							        {	 
							        	var desc = res.responseText;
							        	var content = desc+"<br><a style=\"color: blue; text-decoration: underline; cursor: pointer;\" onclick=\"alterar("+id+",\'"+tipo+"\',"+dpaid+");\">alterar</a>";
							        	div.innerHTML = content;
							        }
							        
							  });
}

<?
if( ( $_REQUEST['alterar'] != '') && ( $forid == 5 ) )
{        		 
        echo' 	verificaFonteRecurso(5);';
}
?> 
</script>