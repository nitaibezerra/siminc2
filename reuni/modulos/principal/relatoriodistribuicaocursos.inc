<?php
/**
 * Sistema Integrado de Monitoramento do Ministério da Educação
 * Setor responsvel: SPO/MEC
 * Desenvolvedor: Desenvolvedores Simec
 * Analistas:  Cristiano Cabral <cristiano.cabral@gmail.com>, Bruno Adann Sagretzki Coura <bruno.coura@mec.gov.br>
 * Programadores: Mário César Gasparini Nascimento <pilpas@gmail.com>
 * Módulo: Reuni
 * Finalidade: Gerar Relatório de consolidação de dados.
 * Data de criação: 03/03/2008
 * Última modificação: 03/03/2008
 */
$modulo = $_REQUEST['modulo'] ;
if( (isset($_REQUEST['xls'])) && ($_REQUEST['xls'] == 1) )
{
	//dump($_SERVER['QUERY_STRING']).die;
	 header('Content-type: application/x-msdownload');
	 //header('Content-type:  text/plain');
	 header('Content-Disposition: attachment; filename=planilha_cursos.xls');
	 header('Pragma: no-cache');
	 header('Expires: 0');
	 

	/*
	 gerando o xml...
	 */

	exibeXSLInstituicao();

	/* script para gerar o xls da função geraXLS
	 
	$relatorio = Array();

	$sql = "SELECT
	nlcid, nlcdesc
	FROM
	reuni.nivellimitecargo";
	$nivel = $db->carregar($sql);
	//adicionando os anos, para cada nível...

	for( $z = 0; $z < count($nivel);  $z++)
	{
	$nivel[$z]['resultado'] = array();
	$sql = "SELECT
	sum(lc.lan2008) as lan2008,
	sum(lc.lan2009) as lan2009,
	sum(lc.lan2010) as lan2010,
	sum(lc.lan2011) as lan2011,
	sum(lc.lan2012) as lan2012,
	cl.nlcid,
	COALESCE(ca.cardesc,'Professor de 3º Grau') as cargo
	FROM
	reuni.lancamentocargo 		 AS lc
	INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
	INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = CAST(lc.calid as integer))
	LEFT  JOIN reuni.cargo		 AS ca ON (ca.carcod = cl.carcod)
	WHERE
	cl.nlcid  in (".$nivel[$z]['nlcid'].")
	GROUP BY cl.nlcid,ca.cardesc
	order by ca.cardesc

	";
	//	dump($sql).die;
	$lancamentos = $db->carregar($sql);
	$nivel[$z]['resultado'] = $lancamentos;
	}
	$relatorio = $nivel;


	echo "Categoria\tCargo Efetivo\t2008\t2009\t2010\t2011\t2012\tTotal\t\n";

	for( $i = 0; $i < count($relatorio);  $i++)
	{
	echo $relatorio[ $i ]['nlcdesc']."\t";

	$total = array();
	$total['lan2008'] = 0;
	$total['lan2009'] = 0;
	$total['lan2010'] = 0;
	$total['lan2011'] = 0;
	$total['lan2012'] = 0;
		
	for( $n = 0; $n < count($relatorio[ $i ]['resultado']);  $n++)
	{
	if($n!=0)

	echo " \t" ;
	echo $relatorio[ $i ]['resultado'][$n]['cargo'] ." \t" ;
	echo $relatorio[ $i ]['resultado'][$n]['lan2008']." \t" ;
	echo $relatorio[ $i ]['resultado'][$n]['lan2009']." \t" ;
	echo $relatorio[ $i ]['resultado'][$n]['lan2010']." \t" ;
	echo $relatorio[ $i ]['resultado'][$n]['lan2011']." \t" ;
	echo $relatorio[ $i ]['resultado'][$n]['lan2012']." \t" ;
	echo ($relatorio[ $i ]['resultado'][$n]['lan2008'] + $relatorio[ $i ]['resultado'][$n]['lan2010'] +
	$relatorio[ $i ]['resultado'][$n]['lan2009'] + $relatorio[ $i ]['resultado'][$n]['lan2011'] +
	$relatorio[ $i ]['resultado'][$n]['lan2012'])." \t\n" ;

	//somando subtotal:
	$total['lan2008'] += $relatorio[ $i ]['resultado'][$n]['lan2008'];
	$total['lan2009'] += $relatorio[ $i ]['resultado'][$n]['lan2009'];
	$total['lan2010'] += $relatorio[ $i ]['resultado'][$n]['lan2010'];
	$total['lan2011'] += $relatorio[ $i ]['resultado'][$n]['lan2011'];
	$total['lan2012'] += $relatorio[ $i ]['resultado'][$n]['lan2012'];

	}

	echo "\tTotal\t" ;
	echo $total['lan2008']." \t" ;
	echo $total['lan2009']." \t" ;
	echo $total['lan2010']." \t" ;
	echo $total['lan2011']." \t" ;
	echo $total['lan2012']." \t" ;

	echo ($total['lan2008'] +  $total['lan2009'] +  $total['lan2010'] +  $total['lan2011'] +  $total['lan2012'])."\t\n\n";
		
		
	}
	die();
    */
}

/*
 echo "<br><br><br><br><br><br><br><br>";
 echo "<pre>";
 print_r($_SERVER);
 */
?>
<?php include APPRAIZ . "includes/cabecalho.inc"; ?>
<br />
<?php
monta_titulo( 'Relatório - Plano de Distribuição de Bolsas', '' );
?>

<form method="POST" name="formulario" id="formulario">
<input type="hidden" id="xls" name="xls">
<input type="hidden" id="enviado" name="enviado">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td align='right' width='80' class="SubTituloDireita">Unidade</td>
		<td width='550'><?
		$sql = " SELECT
                                u.unicod, 
                                u.unidsc 
                            from 
                                public.unidade  as u
                            where  
                                u.unistatus = 'A' and  u.unitpocod = 'U' and u.gunid = 3
                            ORDER BY  u.unidsc";

		$resultado = $db->carregar($sql);
		?> <select id='unicod' size='6' multiple name='unicod[]' >
		<?
		if(is_array($resultado))
		{
			for($i=0;  $i  < count($resultado); $i++)
			{				
				echo "<option ";	
				for( $n = 0; $n < count( $_REQUEST['unicod'] );  $n++){
					if($_REQUEST['unicod'][$n] == $resultado[$i]['unicod']){			
					echo " selected='selected' ";
					}				
				}
				echo " value='".$resultado[$i]['unicod']."'>".$resultado[$i]['unicod'].'  - '.$resultado[$i]['unidsc']."</option>";
			}

		}
		?>
		   </select> <input type="checkbox" name="pilpas" id="pilpas"
			onclick="selecionarTodas();" /> <label for='pilpas'>Selecionar Tudo</label>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td colspan="2" valign="top" align="right">	

		<center>
			<? 
				if($_REQUEST['enviado'] == 'true'){
					echo("<input type='button' class='botao' name='xls' id='xls' value='Gerar XLS' onclick='funtionExportarRel();' />");
				}
			?>
			 
			<input type='button' class="botao" name='consultar' value='consultar' onclick="enviar();" /></center>
		</td>

	</tr>
</table>
</form>
<br>
<p><?php
/*
 echo "<pre>";
 print_r($_REQUEST);
 */


if(isset($_REQUEST['unicod']))
{
	if(!isset($_REQUEST['agrupamento']) || $_REQUEST['agrupamento']=='instituicao')
	{
		exibeTelaInstituicao();
	}

	if(isset($_REQUEST['agrupamento']) && $_REQUEST['agrupamento']=='campi')
	{
		exibeTelaCampi();
	}

}


if(isset($_REQUEST['agrupamento']) && $_REQUEST['agrupamento']=='cargo')
{
	exibeTelaCargo();
}



function exibeXSLInstituicao()
{
	global $db;

	echo "Instituição", "\t";
	echo "Curso", "\t";
	echo "Provimentos Autorizados/Ano", "\t";
	echo "", "\t";
	echo "", "\t";
	echo "", "\t";
	echo "", "\n";

	echo "", "\t";
	echo "", "\t";
	echo "2008", "\t";
	echo "2009", "\t";
	echo "20010", "\t";
	echo "20011", "\t";
	echo "20012", "\n";


	$relatorio = Array();
	//$_REQUEST['unicod'][0] = 26264;
	//dump($_REQUEST);
	for( $i = 0; $i < count( $_REQUEST['unicod'] );  $i++)
	{
		//pra pegar o nome da universidade.
		$sql = "SELECT unidsc FROM unidade WHERE unicod = '" . $_REQUEST['unicod'][$i] . "'";
		$unidsc = $db->pegaUm($sql);


		$relatorio[$_REQUEST['unicod'][$i] . ' - ' . $unidsc] = Array();
		$relatorio[$_REQUEST['unicod'][$i] . ' - ' . $unidsc]['unicod']   =   $_REQUEST['unicod'][$i];

		// carregando os níveis, e cargos
		$sql = "SELECT
                    nlcid, nlcdesc
                FROM
                    reuni.nivellimitecargo
		    where nlcid in (4,5)";
		$nivel = $db->carregar($sql);

		//adicionando os anos, para cada nível...
		for( $z = 0; $z < count($nivel);  $z++)
		{
			$nivel[$z]['resultado'] = array();

			$sql = "SELECT
                            cl.lan2008 as lan2008,
                            cl.lan2009 as lan2009,
                            cl.lan2010 as lan2010,
                            cl.lan2011 as lan2011, 
                            cl.lan2012 as lan2012,
                            cl.nlcid,
			    sc.curnome,
			    sc.curcod,
			    nl.nlcdesc
                        FROM
                            reuni.cursolancamento 		 AS cl
                            INNER JOIN reuni.solicitacaocurso AS sc ON (cl.curid  = sc.curid)
			    			INNER JOIN reuni.nivellimitecargo AS nl ON (nl.nlcid  = cl.nlcid)
			    			INNER JOIN reuni.limitecargo AS lc ON (lc.nlcid = nl.nlcid)
                        WHERE
                        	sc.unicod      =  '".$_REQUEST['unicod'][$i]."'
			    	and cl.nlcid   in ('".$nivel[$z]['nlcid']."')
			    	and lc.limtpreg in ('D', 'M')
                        GROUP BY
				cl.nlcid,sc.curnome,nl.nlcdesc, sc.curcod, cl.lan2008, cl.lan2009, cl.lan2010, cl.lan2011, cl.lan2012
			ORDER BY
				sc.curnome ";			
			$lancamentos = $db->carregar($sql);
			
			$nivel[$z]['resultado'] = $lancamentos;
		}
		$relatorio[$_REQUEST['unicod'][$i] . ' - '.$unidsc]['nivel']   = $nivel;
	}

	$instituicao = array_keys($relatorio);

	for( $i = 0; $i < count($relatorio);  $i++)
	{
			
		//nome codigo - instituição
		echo $instituicao[$i] , "\t" ;
			
		$niveis  =  array_keys($relatorio[$instituicao[$i]]['nivel']);

		$total = array();
		$total['lan2008'] = 0;
		$total['lan2009'] = 0;
		$total['lan2010'] = 0;
		$total['lan2011'] = 0;
		$total['lan2012'] = 0;
			
		for( $n = 0; $n < count($niveis);  $n++)
		{
			//array_keys($relatorio[$instituicao[$i]]['nivel']['']);
			// $cargoValores  =  array_keys($relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['resultado']);

			$resultado = $relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['resultado'];

			$cargoSpan = count($resultado);

			//cargo...
			if($n != 0){
				echo "", "\t";
			}
			echo $relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['nlcdesc'], "\t";
			echo "", "\t";
			echo "", "\t";
			echo "", "\t";
			echo "", "\t";
			echo "", "\n";


			/*
			 danado dos valores!
			 */
			//  $niveis  =  array_keys($relatorio[$instituicao[$i]]['nivel']);
			$subTotal = array();
			$subTotal['lan2008'] = 0;
			$subTotal['lan2009'] = 0;
			$subTotal['lan2010'] = 0;
			$subTotal['lan2011'] = 0;
			$subTotal['lan2012'] = 0;

			for( $r = 0; $r < count($resultado);  $r++)
			{
				//array_keys($relatorio[$instituicao[$i]]['nivel']['']);
				// $cargoValores  =  array_keys($relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['resultado']);
					
				$resultado = $relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['resultado'];

				//exibindo resultado:

				echo "", "\t";
				echo "".$resultado[$r]['curcod']." - ".$resultado[$r]['curnome']."", "\t";
				echo "".$resultado[$r]['lan2008']."" , "\t";
				echo "".$resultado[$r]['lan2009']."" , "\t";
				echo "".$resultado[$r]['lan2010']."" , "\t";
				echo "".$resultado[$r]['lan2011']."" , "\t";
				echo "".$resultado[$r]['lan2012']."" , "\n";

				//somando subtotal:
				$subTotal['lan2008'] += $resultado[$r]['lan2008'];
				$subTotal['lan2009'] += $resultado[$r]['lan2009'];
				$subTotal['lan2010'] += $resultado[$r]['lan2010'];
				$subTotal['lan2011'] += $resultado[$r]['lan2011'];
				$subTotal['lan2012'] += $resultado[$r]['lan2012'];
					
				$total['lan2008'] += $resultado[$r]['lan2008'];
				$total['lan2009'] += $resultado[$r]['lan2009'];
				$total['lan2010'] += $resultado[$r]['lan2010'];
				$total['lan2011'] += $resultado[$r]['lan2011'];
				$total['lan2012'] += $resultado[$r]['lan2012'];					
					
			}
			//exibindo subtotal:			
			echo "", "\t";
			echo "Subtotal", "\t";
			echo"".$subTotal['lan2008']."" , "\t";
			echo"".$subTotal['lan2009']."" , "\t";
			echo"".$subTotal['lan2010']."" , "\t";
			echo"".$subTotal['lan2011']."" , "\t";
			echo"".$subTotal['lan2012']."" , "\n";
		}
		//exibindo total:
		echo "", "\t";
		echo "Total", "\t";
		echo"".$total['lan2008']."" , "\t";
		echo"".$total['lan2009']."" , "\t";
		echo"".$total['lan2010']."" , "\t";
		echo"".$total['lan2011']."" , "\t";
		echo"".$total['lan2012']."" , "\n";
	}
	die;
}

function exibeTelaInstituicao()
{
	global $db;

	$relatorio = Array();


	for( $i = 0; $i < count( $_REQUEST['unicod'] );  $i++)
	{
		// pra pegar o nome da universidade.
		$sql = "SELECT unidsc FROM unidade WHERE unicod = '" . $_REQUEST['unicod'][$i] . "'";
		$unidsc = $db->pegaUm($sql);

		$relatorio[$_REQUEST['unicod'][$i] . ' - ' . $unidsc] = Array();
		$relatorio[$_REQUEST['unicod'][$i] . ' - ' . $unidsc]['unicod']   =   $_REQUEST['unicod'][$i];

		// carregando os níveis, e cargos
		$sql = "SELECT
                    nlcid, nlcdesc
                FROM
                    reuni.nivellimitecargo
		    where nlcid in (4,5)";
		$nivel = $db->carregar($sql);
		//adicionando os anos, para cada nível...
		for( $z = 0; $z < count($nivel);  $z++)
		{

			// for( $w = 0; $w < count($_REQUEST['ano']);  $w++)
			//  {
			// array somatório para cada ano...
			//temque carregar os cargos para o nivellimit
			$nivel[$z]['resultado'] = array();

			/*
			 echo "<br><br>nivel:";
			 print_r($nivel[$z]);
			 echo "<hr>";
			 */
			$sql = "SELECT
                            sum(distinct(cl.lan2008)) as lan2008,
                            sum(distinct(cl.lan2009)) as lan2009,
                            sum(distinct(cl.lan2010)) as lan2010,
                            sum(distinct(cl.lan2011)) as lan2011, 
                            sum(distinct(cl.lan2012)) as lan2012,
                            cl.nlcid,
			    sc.curnome,
			    sc.curcod,
			    nl.nlcdesc
                        FROM
                            reuni.cursolancamento 		 AS cl
                            INNER JOIN reuni.solicitacaocurso AS sc ON (cl.curid  = sc.curid)
			    			INNER JOIN reuni.nivellimitecargo AS nl ON (nl.nlcid  = cl.nlcid)
			    			INNER JOIN reuni.limitecargo AS lc ON (lc.nlcid = nl.nlcid)
                        WHERE
                        	sc.unicod      =  '".$_REQUEST['unicod'][$i]."'
			    			and cl.nlcid   in ('".$nivel[$z]['nlcid']."')
			    			and lc.limtpreg in ('D', 'M')
                        GROUP BY
				cl.nlcid,sc.curnome,nl.nlcdesc, sc.curcod
			ORDER BY
				sc.curnome ";
	
			$lancamentos = $db->carregar($sql);			
			$nivel[$z]['resultado'] = $lancamentos;
		}
		$relatorio[$_REQUEST['unicod'][$i] . ' - '.$unidsc]['nivel']   = $nivel;
	}
	?>
<table width="95%" align="center" border="0" cellspacing="0"
	cellpadding="0" class="listagem">
	<thead>
		<tr>
			<td rowspan="2" valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Instituição</strong></td>
			<td rowspan="2" width='200' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Curso</strong></td>
			<td colspan="5" valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center><strong>Provimentos Autorizados/Ano</strong></center>
			</td>

		</tr>
		<tr>
			<td width='100' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>2008</center>
			</td>
			<td width='100' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>2009</center>
			</td>
			<td width='100' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>2010</center>
			</td>
			<td width='100' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>2011</center>
			</td>
			<td width='100' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>2012</center>
			</td>
		</tr>
	</thead>
	<tbody>
	<?
	$instituicao = array_keys($relatorio);
	for( $i = 0; $i < count($relatorio);  $i++)
	{
		?>
		<tr bgcolor="<? echo $marcado; ?>">
			<td valign=top class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				<?
				//nome codigo - instituição
				echo $instituicao[$i];
				?></td>

			<td valign="top" width='700' colspan="6"
				style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">

			<table align="center" border="0" cellspacing="0" cellpadding="2"
				style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">
				<?
				$niveis  =  array_keys($relatorio[$instituicao[$i]]['nivel']);

				$total = array();
				$total['lan2008'] = 0;
				$total['lan2009'] = 0;
				$total['lan2010'] = 0;
				$total['lan2011'] = 0;
				$total['lan2012'] = 0;
					
				for( $n = 0; $n < count($niveis);  $n++)
				{
					//array_keys($relatorio[$instituicao[$i]]['nivel']['']);
					// $cargoValores  =  array_keys($relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['resultado']);

					$resultado = $relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['resultado'];

					$cargoSpan = count($resultado);



					?>
				<tr bgcolor="#EFEFEF">
					<td colspan="6" valign="top" class="title"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<?
						//cargo...
						echo $relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['nlcdesc'];
						//echo "<b>".$relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['nlcdesc']."</b>";
						?></td>
				</tr>
				<?
				/*
				 danado dos valores!
				 */
				//  $niveis  =  array_keys($relatorio[$instituicao[$i]]['nivel']);
				$subTotal = array();
				$subTotal['lan2008'] = 0;
				$subTotal['lan2009'] = 0;
				$subTotal['lan2010'] = 0;
				$subTotal['lan2011'] = 0;
				$subTotal['lan2012'] = 0;				
				
				
				for( $r = 0; $r < count($resultado);  $r++)
				{
					//array_keys($relatorio[$instituicao[$i]]['nivel']['']);
					// $cargoValores  =  array_keys($relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['resultado']);

					$resultado = $relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['resultado'];
					?>
				<tr>
					<td width='210'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><? echo $resultado[$r]['curcod']." - ".$resultado[$r]['curnome'] ;?></td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $resultado[$r]['lan2008']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $resultado[$r]['lan2009']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $resultado[$r]['lan2010']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $resultado[$r]['lan2011']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $resultado[$r]['lan2012']; ?></center>
					</td>
				</tr>
				<?
				//somando subtotal:
					
				$subTotal['lan2008'] += $resultado[$r]['lan2008'];
				$subTotal['lan2009'] += $resultado[$r]['lan2009'];
				$subTotal['lan2010'] += $resultado[$r]['lan2010'];
				$subTotal['lan2011'] += $resultado[$r]['lan2011'];
				$subTotal['lan2012'] += $resultado[$r]['lan2012'];
					
					
					
				$total['lan2008'] += $resultado[$r]['lan2008'];
				$total['lan2009'] += $resultado[$r]['lan2009'];
				$total['lan2010'] += $resultado[$r]['lan2010'];
				$total['lan2011'] += $resultado[$r]['lan2011'];
				$total['lan2012'] += $resultado[$r]['lan2012'];
					

					
}
?>
				<tr>
					<td width='210'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;"><b>SubTotal</b></td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $subTotal['lan2008']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $subTotal['lan2009']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $subTotal['lan2010']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $subTotal['lan2011']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $subTotal['lan2012']; ?></center>
					</td>
				</tr>
				<?
					
}
?>
				<tr>
					<td width='210'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;"><b>Total</b></td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2008']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2009']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2010']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2011']; ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2012']; ?></center>
					</td>
				</tr>

			</table>

			</td>
		</tr>
		<?
}
?>
	</tbody>
</table>
<?
}
?>

<script type="text/javascript">

    function enviar()
    {
	  var unicod = document.getElementById('unicod');
	  document.getElementById('xls').value = 0; 
	  document.getElementById('enviado').value = true;
	    
	  if (unicod.selectedIndex =='-1') 
	  {
	    alert('Selecione ao menos uma Unidade');
	    return false;
	  }  
	    
	  formulario.submit();
    }

    
    function validaAgrupamento()
    {
	    
	//    var cargo = document.getElementById('cargo');
	    var unicod = document.getElementById('unicod');
	    var xls = document.getElementById('xls');

	    if (cargo.checked)
	    {
		unicod.disabled = true;
		xls.style.display="block";
	    }
	    else
	    {
		unicod.disabled = false;
		xls.style.display="none";
	    }
	    
    }
    
    function funtionExportarRel()
    {
      document.getElementById('xls').value = 1; 
	 /*
	  var unicod = document.getElementById('unicod');	  
	  if (unicod.selectedIndex =='-1') 
	  {
	    alert('Selecione ao menos uma Unidade');
	    return false;
	  }*/	 	  
	  formulario.submit();
    }
    
    
    function gerarXLS()
    {  
	    window.open('<? echo $_SERVER['SCRIPT_URI']."reuni.php?".$_SERVER['QUERY_STRING']."&xls=true"; ?>');
    }
    
    function selecionarTodas ()
    {
	/*
	var cargo = document.getElementById('cargo');
	var pilpas = document.getElementById('pilpas');
	if (cargo.checked) {
	    pilpas.checked = false;
	    return false;
	}
	*/
        var tudo = document.getElementById('unicod');

	for (var i = 0; i< tudo.options.length; i++) 
	{
	   tudo.options[i].selected = true;
	}

    }
</script>





