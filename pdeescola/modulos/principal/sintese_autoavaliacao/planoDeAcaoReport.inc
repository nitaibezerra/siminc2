<?php
# Verfica se o pdeid e intid estão na sessão
pde_verificaSessao();
#planoDeAcao.inc
 //SELECT pseidpai, psedescricao FROM pdeescola.planosuporteestrategico WHERE pseid
/*----------------- Configurações de Página ---------------------*/ 


//if( $plagerenteplanoacao == '' )
//{
//	echo("<script>alert('Para visualizar esta pagina e necessário ter informado um Gerente para o Plano de Ação'); \n</script>");	
//	echo("<script> history.go(-1)</script>");
//	exit;
//}
$pseid = $_REQUEST['pseid'];

if( $_REQUEST['pseid'] != '' )
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
	
	$idPai = $db->pegaUm( $pegaPai );
	
	$pegaMeta = "SELECT psedescricao FROM
	         	pdeescola.planosuporteestrategico where pseid = '{$_REQUEST['pseid']}'";
	
	$meta = $db->pegaUm( $pegaMeta );
	
	if( $idPai != '' )
	{
	$sqlEstrategia = "SELECT 
						pseidpai, 
						psedescricao 
					  FROM
					  	pdeescola.planosuporteestrategico
					  WHERE
					  	pseid = '{$idPai}'";
					  	
	$rsDadosEstrategia = $db->carregar( $sqlEstrategia );
	
		if( $rsDadosEstrategia ) {
			$sqlObjetivo = "SELECT 
								pseidpai, 
								psedescricao 
							  FROM
							  	pdeescola.planosuporteestrategico
							  WHERE
							  	pseid = '{$rsDadosEstrategia[0]['pseidpai']}'";
							  	
			$rsDadosObjetivo = $db->carregar( $sqlObjetivo );
		}
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
	exit;
}

if( $_REQUEST['alterar'] != '')
{
	$dpaid = $_REQUEST['alterar'];
	//$pseid = $_REQUEST['pseid'];

	
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
	
	$dpavalorcapital = number_format($dpavalorca,2,',','.');
	$dpavalorcusteio = number_format($dpavalorcust,2,',','.');

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


//query que carrega os dados 
 

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Plano de Ação ', '');
echo cabecalhoPDE(); 

?>
		<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
   
			<tr>
				<td class="subtitulodireita" width="20%">
				<b>	Objetivo Estratégico:</b>
				</td> 
				<td align="left">
					<?php
						echo $objetivo;
					?>
                	</td>
                </tr>	
                 	<tr>
					<td class="subtitulodireita">
					<b>	Líder do Objetivo:</b>
					</td> 
					<td align="left">
						<?php
						echo $plaliderobjetivo;
					?>
                	</td>
                </tr>	
 				<tr>
					<td class="subtitulodireita">
					<b>	Estratégia:</b>
					</td> 
					<td align="left">
						 <?php
						echo $estrategia;
					?>
                	</td>
                </tr>	 
                <tr>
					<td class="subtitulodireita">
					<b>	Meta:</b>
					</td> 
					<td align="left">
						 <?php
						echo $meta;
					?>
                	</td>
                </tr>	 
				<tr>
					<td class="subtitulodireita">
					<b>	Indicador da Meta:</b>
					</td> 
					<td align="left">
						<?php
						echo $plaindicadormeta;
					?>
                	</td>
                </tr>	
                <tr>
					<td class="subtitulodireita">
					<b>	Gerente de Plano de Ação:</b>
					</td> 
					<td align="left">
						 <?php
						echo $plagerenteplanoacao;
					?>
                	</td>
                </tr>	
                <tr>
					<td class="subtitulodireita">
					<b>	Início:</b>
					</td> 
					<td align="left">
						 <?php
						echo $pladatainicio;
					?>
                	</td>
                </tr>	
                <tr>
					<td class="subtitulodireita">
					<b>	Revisão:</b>
					</td> 
					<td align="left">
						 <?php
						echo $pladatarevisao;
					?>
                	</td>
                </tr>	
                <tr>
					<td class="subtitulodireita">
					<b>	Data Término:</b>
					</td> 
					<td align="left">
						 <?php
						echo $pladatatermino;
					?>
                	</td>
                </tr>	
             </table>

<?php
 
/*----------------- Recupera dados ---------------------*/  
$dados = $db->pegaLinha("SELECT 	pdeuf,
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

<form onSubmit="return validaForm(this);"action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario" >

</form>


<?php
        	$sql = "
        			SELECT   
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
?>
<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
	<td title="Descrição">
		<b><?= $i+1 ?></b>
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
		<?=$dados[$i]['tprdescricao']?>
	</td>
	<td title="Categoria">
		<?=$dados[$i]['tcadescricao']?>
	</td>
</tr>
<?php
}
?>
</tbody>
</table>
 
<table align="center" width="95%">  
<?echo $cForm ->montarbuttons('');  ?>
</table>	

<script>


function verificaFonteRecurso(id)
{ 
	var id;
	
	if( id == 5 )
	{
		document.getElementById('tprid').disabled = 0;
		document.getElementById('tcaid').disabled = 0;
		top.obrigatorio = 'sim';
	}
	else
	{
		document.getElementById('tprid').disabled = 1;
		document.getElementById('tcaid').disabled = 1;
		top.obrigatorio = 'nao';
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
			document.getElementById('dpavalorcusteio').disabled = 1;
			top.capitalObrigatorio = 'verdadeiro';
		}
		else
		{
			document.getElementById('dpavalorcapital').disabled = 0;
		}
	}
	else
	{
		if( name == 'dpavalorcusteio')
		{
			var tam2 = document.formulario.dpavalorcusteio.value.length;
			
			if( tam2 > 0 )
			{
				document.getElementById('dpavalorcapital').disabled = 1;
				top.CusteiolObrigatorio = 'verdadeiro';
			}
			else
			{
				document.getElementById('dpavalorcusteio').disabled = 0;
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
			top.CapitallObrigatorio = 'false';
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
				top.CusteiolObrigatorio = 'false';
			}
		}
	}	
		
}

function alterarDetalhe( dpaid )
{
	return window.location.href = 'pdeescola.php?'+"<?php echo $_SERVER['argv'][0]; ?>"+'&alterar='+dpaid;
}
//pdeescola.php?modulo=principal/sintese_autoavaliacao/problemas_causas_acoes&acao=A
    
</script>