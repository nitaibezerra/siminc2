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

if( (isset($_REQUEST['exportar_xls'])) && ($_REQUEST['exportar_xls'] == 1) )
{

	header('Content-type: application/x-msdownload');
	//header('Content-type:  text/plain');
	header('Content-Disposition: attachment; filename=planilha_cargos.xls');
	header('Pragma: no-cache');
	header('Expires: 0');

	exibeXSLInstituicao();
}

if( (isset($_REQUEST['rel_consolidado'])) && ($_REQUEST['rel_consolidado'] == 1) )
{

	header('Content-type: application/x-msdownload');
	//header('Content-type:  text/plain');
	header('Content-Disposition: attachment; filename=planilha_cargos_consolidado.xls');
	header('Pragma: no-cache');
	header('Expires: 0');

	exibeRelConsolidado();
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
monta_titulo( 'Relatório - Plano de distribuição de Cargos', '' );
?>

<form method="POST" name="formulario" id="formulario"><input
	type="hidden" id="exportar_xls" name="exportar_xls" value="false"> <input
	type="hidden" id="enviado" name="enviado">
	<input type="hidden" id="rel_consolidado" name="rel_consolidado" value="false">
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
		?> <select id='unicod' size='6' multiple name='unicod[]'>
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


		<td width='80' class="SubTituloDireita">Agrupamento</td>
		<td>
			<input type="radio" name="agrupamento" id="instituicao"
			value="instituicao" checked='checked' onchange="validaAgrupamento();" />
			<label for='instituicao'>Instituição</label> 
			<input type="radio"
			name="agrupamento" id="campi" value="campi"
			onchange="validaAgrupamento();" /> <label for='campi'>Campi</label> 
			<input	type="radio" name="agrupamento" id="cargo" value="cargo"
			onchange="validaAgrupamento();" /> <label for='cargo'>Consolidado</label>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td colspan="2" valign="top" align="right"><? 
		if($_REQUEST['enviado'] == 'true'){
			$exibir = "style='display: inline'";
			//echo("<input type='button' class='botao' name='bt_xls' id='bt_xls' value='Gerar XLS' onclick='funtionExportarRel();' />");
		}else  $exibir = "style='display: none'";
		?> 
		<input type='button' class='botao' name='bt_xls' id='bt_xls' <?=$exibir ?> value='Gerar XLS' onclick='funtionExportarRel();' />
		<input type='button' class="botao" name='consultar' id="consultar"
			value='consultar' onclick="enviar();" /></td>
		<td>&nbsp;</td>
		<td valign="top">
		<center><input type='button' class="botao" name='xls' id='xls'
			value='Gerar XLS Consolidado' onclick="gerarXLS();" style="display: none" /></center>
		</td>
	</tr>
	<tr></tr>
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


function exibeRelConsolidado(){
	
	global $db;

	//script para gerar o xls da função geraXLS
	$relatorio = Array();

		$sql = "SELECT
                    nlcid, nlcdesc
                FROM
                reuni.nivellimitecargo
                where nlcid not in (4,5)";
	$nivel = $db->carregar($sql);
	//adicionando os anos, para cada nível...
	

	for( $z = 0; $z < count($nivel);  $z++)
	{
		$nivel[$z]['resultado'] = array();
		$sql = "
		SELECT  sum(lan2008) as lan2008, 
			sum (lan2009)  as lan2009, 
			sum (lan2010)  as lan2010, 
			sum(lan2011)  as lan2011 , 
			sum(lan2012)  as lan2012,
		        nlcid,
		        cargo
		FROM(
			SELECT
			sum(lc.lan2008) as lan2008,
			sum(lc.lan2009) as lan2009,
			sum(lc.lan2010) as lan2010,
			sum(lc.lan2011) as lan2011,
			sum(lc.lan2012) as lan2012,
			cl.nlcid,
			COALESCE(ca.cardesc,'Professor de 3º Grau') as cargo
			FROM
			reuni.lancamentocargo 	AS lc
			INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
			INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = CAST(lc.calid as integer))
			LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
			WHERE
			cl.nlcid  in (".$nivel[$z]['nlcid'].")
			GROUP BY cl.nlcid,ca.cardesc
			ORDER BY ca.cardesc
			) as foo
		GROUP BY nlcid, cargo
		";

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

}

function exibeXSLInstituicao()
{
	global $db;

	echo "Instituição", "\t";
	echo "Cargo/Nivel", "\t";
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
                    where nlcid not in (4,5)";	
		$nivel = $db->carregar($sql);
		//adicionando os anos, para cada nível...
		for( $z = 0; $z < count($nivel);  $z++)
		{
			$nivel[$z]['resultado'] = array();


			$sql = "
			SELECT  sum(lan2008) as lan2008, 
				sum (lan2009)  as lan2009, 
				sum (lan2010)  as lan2010, 
				sum(lan2011)  as lan2011 , 
				sum(lan2012)  as lan2012,
			        nlcid,
			        cargo
			FROM(
				SELECT
	                            lc.lan2008 as lan2008,
	                            lc.lan2009 as lan2009,
	                            lc.lan2010 as lan2010,
	                            lc.lan2011 as lan2011, 
	                            lc.lan2012 as lan2012,
	                            cl.nlcid,
	                            COALESCE(ca.cardesc,'Docente') as cargo
	                        FROM
	                            reuni.lancamentocargo 	AS lc
	                            INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
	                            INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = CAST(lc.calid as integer))
	                            LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
	                           
	                        WHERE
	                            cu.unicod      =  '".$_REQUEST['unicod'][$i]."'
	                            and cl.nlcid   in ('".$nivel[$z]['nlcid']."') 
	                            and cl.limctpreg = 'C'
	                        GROUP BY cl.nlcid,ca.cardesc, lc.lan2008, lc.lan2009, lc.lan2010, lc.lan2011, lc.lan2012
							ORDER BY ca.cardesc 
			) as foo
			GROUP BY nlcid, cargo
				";
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
			 valores!
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
				$resultado = $relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['resultado'];
					
				//exibindo resultado:

				echo "", "\t";
				echo "".$resultado[$r]['cargo']."", "\t";
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

	die();

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
                    where nlcid not in (4,5)";		

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
			$sql = "
					SELECT  sum(lan2008) as lan2008, 
					sum (lan2009)  as lan2009, 
					sum (lan2010)  as lan2010, 
					sum(lan2011)  as lan2011 , 
					sum(lan2012)  as lan2012,
			        nlcid,
			        cargo
					FROM(	
						SELECT 
                            sum(lc.lan2008) as lan2008,
                            sum(lc.lan2009) as lan2009,
                            sum(lc.lan2010) as lan2010,
                            sum(lc.lan2011) as lan2011, 
                            sum(lc.lan2012) as lan2012,
                            cl.nlcid,
                            COALESCE(ca.cardesc,'Docente') as cargo
                        FROM
                            reuni.lancamentocargo 	AS lc
                            INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
                            INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = CAST(lc.calid as integer))
                            LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)                           
                        WHERE
                            cu.unicod      =  '".$_REQUEST['unicod'][$i]."'
                            and cl.nlcid   in ('".$nivel[$z]['nlcid']."') 
                            and cl.limctpreg = 'C'
                        GROUP BY cl.nlcid,ca.cardesc, lc.lan2008, lc.lan2009, lc.lan2010, lc.lan2011, lc.lan2012
						ORDER BY ca.cardesc
						) as foo
					GROUP BY nlcid, cargo
						";
			$lancamentos = $db->carregar($sql);
			
			//dump($sql);

			$nivel[$z]['resultado'] = $lancamentos;

			// }
		}
		$relatorio[$_REQUEST['unicod'][$i] . ' - '.$unidsc]['nivel']   = $nivel;

		//  $relatorio[$_REQUEST['unicod'][$i] . ' - '.$unidsc]['ano']     = $_REQUEST['ano'];

	}
	/*
	 echo "<pre>";
	 print_r($relatorio);
	 echo "</pre>";
	 */
	?>
<table width="95%" align="center" border="0" cellspacing="0"
	cellpadding="0" class="listagem">
	<thead>
		<tr>
			<td rowspan="2" valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Instituição</strong></td>
			<td rowspan="2" width='200' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Cargo/Nivel</strong></td>
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
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><? echo $resultado[$r]['cargo'] ;?></td>
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


/*
 *   desenha tela agrupado por campus.
 */

function exibeTelaCampi()
{
	global $db;

	$relatorio = Array();
	for( $i = 0; $i < count( $_REQUEST['unicod'] );  $i++)
	{
		// pra pegar o nome da universidade.
		$sql = "SELECT unidsc FROM unidade WHERE unicod = '".$_REQUEST['unicod'][$i] ."'";
		$unidsc = $db->pegaUm($sql);

		$relatorio[$_REQUEST['unicod'][$i] . ' - '.$unidsc] = Array();
		$relatorio[$_REQUEST['unicod'][$i] . ' - '.$unidsc]['unicod']   =   $_REQUEST['unicod'][$i];

		// carregar cada campi
		$sql = "SELECT cauid, caudesc
                FROM reuni.campusuniversitario
                WHERE unicod = '".$_REQUEST['unicod'][$i]."'";
		$campi = $db->carregar($sql);


		$relatorio[$_REQUEST['unicod'][$i] . ' - '.$unidsc]['campi']   =  $campi;

		for( $c = 0; $c < count($campi);  $c++)
		{

			$sql = "SELECT
                        nlcid, nlcdesc
                    FROM
                        reuni.nivellimitecargo";
			$nivel = $db->carregar($sql);
			//adicionando os anos, para cada nível...

			for( $z = 0; $z < count($nivel);  $z++)
			{
				//por algum motivo do além, o php, está passando errado o índice $c....
				//quando tem várias universidades..
				if ($campi[$c]['cauid'] =="")
			 continue;
			 	
			 $nivel[$z]['resultado'] = array();
			 $sql = "
			 
				SELECT  sum(lan2008) as lan2008, 
					sum (lan2009)  as lan2009, 
					sum (lan2010)  as lan2010, 
					sum(lan2011)  as lan2011 , 
					sum(lan2012)  as lan2012,
				        nlcid,
				        cargo
				FROM(
				 
				 	SELECT
                            sum(lc.lan2008) as lan2008,
                            sum(lc.lan2009) as lan2009,
                            sum(lc.lan2010) as lan2010,
                            sum(lc.lan2011) as lan2011, 
                            sum(lc.lan2012) as lan2012,
                            cl.nlcid,
                            COALESCE(ca.cardesc,'Docente') as cargo
                        FROM
                            reuni.lancamentocargo 	AS lc
                            INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
                            INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = CAST ( lc.calid  as integer ))
                            LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
                        WHERE
                            cu.unicod     =  '".$_REQUEST['unicod'][$i]."'
                            and cl.nlcid  in (".$nivel[$z]['nlcid'].")
                            and cu.cauid  =  ".$campi[$c]['cauid']."
                        GROUP BY cl.nlcid,ca.cardesc
						ORDER BY ca.cardesc 
			) as foo
			GROUP BY nlcid, cargo
			";

			 $lancamentos = $db->carregar($sql);
			 $nivel[$z]['resultado'] = $lancamentos;
			}
			$relatorio[$_REQUEST['unicod'][$i] . ' - '.$unidsc]['campi'][$c]['nivel'] = $nivel;
		}
	}
	?>

<table width="95%" align="center" border="0" cellspacing="0"
	cellpadding="0" class="listagem">
	<thead>
		<tr>
			<td colspan="7" valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center><strong>Universidade</strong></center>
			</td>
		</tr>
		<tr>
			<td rowspan="2" valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Campi</strong></td>
			<td rowspan="2" width='200' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Cargo/Nivel</strong></td>
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
	<br>
	<br>
	<tbody>
	<?
	$instituicao = array_keys($relatorio);


	for( $i = 0; $i < count($relatorio);  $i++)
	{
		?>
		<tr bgcolor="#EFEFEF">
			<td colspan="7" valign="top" class="title"><br>
			<?
			//cargo...
			// echo $relatorio[ $instituicao[$i] ]['campi'][$c]['nivel'][ $niveis[$n] ]['nlcdesc'];
			//echo "<b>".$relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['nlcdesc']."</b>";
			?></td>
		</tr>

		<tr>
			<td colspan="7" valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center><strong> <?
			//nome codigo - instituição
			echo $instituicao[$i];
			?> </strong></center>
			</td>
		</tr>
		<?

		$keys_campi = array_keys($relatorio[$instituicao[$i]]['campi']);
		/*
		 echo"<pre>";
		 print_r($keys_campi);

		 die();
		 */
		for( $c = 0; $c <  count($keys_campi);  $c++)
		{
			?>

		<tr bgcolor="<? echo $marcado; ?>">
			<td valign=top class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				<?
				echo $relatorio[$instituicao[$i]]['campi'][$c]['caudesc'];
				?></td>

			<td valign="top" width='700' colspan="6"
				style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">

			<table align="center" border="0" cellspacing="0" cellpadding="2"
				style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">
				<?

				$total = array();
				$total['lan2008'] = 0;
				$total['lan2009'] = 0;
				$total['lan2010'] = 0;
				$total['lan2011'] = 0;
				$total['lan2012'] = 0;
					
				$niveis  =  array_keys($relatorio[$instituicao[$i]]['campi'][$c]['nivel']);
				for( $n = 0; $n < count($niveis);  $n++)
				{
					?>
				<tr bgcolor="#EFEFEF">
					<td colspan="6" valign="top" class="title"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<?
						//cargo...
						echo $relatorio[ $instituicao[$i] ]['campi'][$c]['nivel'][ $niveis[$n] ]['nlcdesc'];
						//echo "<b>".$relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['nlcdesc']."</b>";
						?></td>
				</tr>
				<?
				/*
				 danado dos valores!
				 */
				$subTotal = array();
				$subTotal['lan2008'] = 0;
				$subTotal['lan2009'] = 0;
				$subTotal['lan2010'] = 0;
				$subTotal['lan2011'] = 0;
				$subTotal['lan2012'] = 0;

				$resultado = $relatorio[ $instituicao[$i] ]['campi'][$c]['nivel'][ $niveis[$n] ]['resultado'];
				for( $r = 0; $r < count($resultado);  $r++)
				{
					?>
				<tr>
					<td width='210'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><? echo $resultado[$r]['cargo'] ;?></td>
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
					<center><? echo $total['lan2008'];  ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2009'];  ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2010'];  ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2011'];  ?></center>
					</td>
					<td width='105'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2012'];  ?></center>
					</td>
				</tr>

			</table>

			</td>
		</tr>
		<tr bgcolor="#EFEFEF">
			<td colspan="7" valign="top" class="title"><br>
			<?
			//cargo...
			// echo $relatorio[ $instituicao[$i] ]['campi'][$c]['nivel'][ $niveis[$n] ]['nlcdesc'];
			//echo "<b>".$relatorio[ $instituicao[$i] ]['nivel'][ $niveis[$n] ]['nlcdesc']."</b>";
			?></td>
		</tr>
		<?
} //campi
}//univer...
?>
	</tbody>
</table>

<?

}//function






/*
 *   desenha tela agrupado por cargo
 */

function exibeTelaCargo()
{
	global $db;

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
		$sql = "
		
		SELECT  sum(lan2008) as lan2008, 
			sum (lan2009)  as lan2009, 
			sum (lan2010)  as lan2010, 
			sum(lan2011)  as lan2011 , 
			sum(lan2012)  as lan2012,
		        nlcid,
		        cargo
		FROM(
			SELECT
				    sum(lc.lan2008) as lan2008,
				    sum(lc.lan2009) as lan2009,
				    sum(lc.lan2010) as lan2010,
				    sum(lc.lan2011) as lan2011, 
				    sum(lc.lan2012) as lan2012,
				    cl.nlcid,
				    COALESCE(ca.cardesc,'Professor de 3º Grau') as cargo
				FROM
				    reuni.lancamentocargo 	AS lc
				    INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
				    INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = CAST(lc.calid as integer))		
				    LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
				WHERE
				   -- cu.unicod     =  '".$_REQUEST['unicod'][$i]."' and
				     cl.nlcid  in (".$nivel[$z]['nlcid'].")
				    --and cu.cauid  =  ".$campi[$c]['cauid']."
				GROUP BY cl.nlcid,ca.cardesc 
				ORDER BY ca.cardesc 
			) as foo
			GROUP BY nlcid, cargo			
			";
			
		$lancamentos = $db->carregar($sql);
		$nivel[$z]['resultado'] = $lancamentos;
	}
	$relatorio = $nivel;

	?>

<table width="95%" align="center" border="0" cellspacing="0"
	cellpadding="0" class="listagem">
	<thead>
		<tr>
			<td rowspan="2" align="center" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Categoria</strong></td>
			<td rowspan="2" align="center" width='250' class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Cargo
			Efetivo</strong></td>
			<td width='100' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>2008</center>
			</td>
			<td width='85' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>2009</center>
			</td>
			<td width='85' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>2010</center>
			</td>
			<td width='85' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>2011</center>
			</td>
			<td width='85' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>2012</center>
			</td>
			<td rowspan="2" width='90' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>Total</center>
			</td>
		</tr>
		<tr>
			<td width='85' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>Quantitativo</center>
			</td>
			<td width='85' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>Quantitativo</center>
			</td>
			<td width='85' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>Quantitativo</center>
			</td>
			<td width='85' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>Quantitativo</center>
			</td>
			<td width='85' valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
			<center>Quantitativo</center>
			</td>
		</tr>
	</thead>
	<tbody>
	<?
	for( $i = 0; $i < count($relatorio);  $i++)
	{
		?>
		<tr bgcolor="<? echo $marcado; ?>">
			<td valign=top class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				<?     //nível
				echo "<b>".$relatorio[ $i ]['nlcdesc']."</b>";
				?></td>

			<td valign="top" colspan="7"
				style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">
			<table align="right" border="0" cellspacing="0" cellpadding="2"
				style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">
				<?
				$total = array();
				$total['lan2008'] = 0;
				$total['lan2009'] = 0;
				$total['lan2010'] = 0;
				$total['lan2011'] = 0;
				$total['lan2012'] = 0;

				for( $n = 0; $n < count($relatorio[ $i ]['resultado']);  $n++)
				{
					?>
				<tr>
					<td width='250'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><? echo $relatorio[ $i ]['resultado'][$n]['cargo'] ;
						?></td>
					<td width='100'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $relatorio[ $i ]['resultado'][$n]['lan2008']; ?></center>
					</td>
					<td width='85'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $relatorio[ $i ]['resultado'][$n]['lan2009']; ?></center>
					</td>
					<td width='85'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $relatorio[ $i ]['resultado'][$n]['lan2010']; ?></center>
					</td>
					<td width='85'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $relatorio[ $i ]['resultado'][$n]['lan2011']; ?></center>
					</td>
					<td width='85'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $relatorio[ $i ]['resultado'][$n]['lan2012']; ?></center>
					</td>
					<td width='90'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><?
					echo  (
					$relatorio[ $i ]['resultado'][$n]['lan2008'] +
					$relatorio[ $i ]['resultado'][$n]['lan2009'] +
					$relatorio[ $i ]['resultado'][$n]['lan2010'] +
					$relatorio[ $i ]['resultado'][$n]['lan2011'] +
					$relatorio[ $i ]['resultado'][$n]['lan2012']
					);
					?></center>
					</td>
				</tr>
				<?
				//somando subtotal:
				$total['lan2008'] += $relatorio[ $i ]['resultado'][$n]['lan2008'];
				$total['lan2009'] += $relatorio[ $i ]['resultado'][$n]['lan2009'];
				$total['lan2010'] += $relatorio[ $i ]['resultado'][$n]['lan2010'];
				$total['lan2011'] += $relatorio[ $i ]['resultado'][$n]['lan2011'];
				$total['lan2012'] += $relatorio[ $i ]['resultado'][$n]['lan2012'];
}
?>
				<tr>
					<td width='250'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;"><b>Total</b></td>
					<td width='100'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2008'];  ?></center>
					</td>
					<td width='85'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2009'];  ?></center>
					</td>
					<td width='85'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2010'];  ?></center>
					</td>
					<td width='85'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2011'];  ?></center>
					</td>
					<td width='85'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><? echo $total['lan2012'];  ?></center>
					</td>
					<td width='90'
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<center><?
					echo  (
				 $total['lan2008'] +
				 $total['lan2009'] +
				 $total['lan2010'] +
				 $total['lan2011'] +
				 $total['lan2012']
				 );
				 ?></center>
					</td>
				</tr>

			</table>

			</td>

		</tr>
		<tr>
			<td valign="top" colspan="8"
				style="border-right: 0px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 0px solid #ffffff;">
			<p><br>
			
			</td>
		</tr>
		<?
}//nivel
?>


	</tbody>
</table>
<?
}//function
?>

<script type="text/javascript">

    function enviar ()
    {
	  var cargo = document.getElementById('cargo');
	  var unicod = document.getElementById('unicod');	
	  document.getElementById('enviado').value = true;  	  
	  document.getElementById('exportar_xls').value = 0; 
	  document.getElementById('rel_consolidado').value = 0; 
	    
	  if (!cargo.checked && unicod.selectedIndex =='-1') 
	  {
	    alert('Selecione ao menos uma Unidade');
	    return false;
	  }  
	    
	  formulario.submit();
    }

    
    function validaAgrupamento()
    {
	    var cargo = document.getElementById('cargo');
	    var unicod = document.getElementById('unicod');
	    var xls = document.getElementById('xls');
	    var bt_xls = document.getElementById('bt_xls');
	    var consultar = document.getElementById('consultar');	    

	    if (cargo.checked)
	    {
		unicod.disabled = true;
		xls.style.display="block";
		bt_xls.style.display="none";
		consultar.style.display="none";
		document.getElementById('rel_consolidado').value = 1; 
		document.getElementById('exportar_xls').value = 0; 
	    }
	    else
	    {
		unicod.disabled = false;
		xls.style.display="none";
		//bt_xls.style.display="inline";
		consultar.style.display="inline";
				document.getElementById('rel_consolidado').value = 0; 
		document.getElementById('exportar_xls').value = 1; 
	    }
    }
    
    function funtionExportarRel()
    {
      document.getElementById('exportar_xls').value = 1; 
      
	  var unicod = document.getElementById('unicod');	  
	  /*
	  if (unicod.selectedIndex =='-1') 
	  {
	    alert('Selecione ao menos uma Unidade');
	    return false;
	  }	
	  */ 	  
	  formulario.submit();
    }
    
    function gerarXLS()
    {
     document.getElementById('rel_consolidado').value = 1; 
     formulario.submit();
	  // window.open('<? echo $_SERVER['SCRIPT_URI']."reuni.php?".$_SERVER['QUERY_STRING']."&xls=true"; ?>');
    }
    
    function selecionarTodas ()
    {
	var cargo = document.getElementById('cargo');
	var pilpas = document.getElementById('pilpas');
	if (cargo.checked) {
	    pilpas.checked = false;
	    return false;
	}
	
        var tudo = document.getElementById('unicod');

        if(pilpas.checked == true )
        {
		for (var i = 0; i< tudo.options.length; i++) 
		{
		   tudo.options[i].selected = true;
		}
        }
        else
        {
		for (var i = 0; i< tudo.options.length; i++) 
		{
		   tudo.options[i].selected = false;
		}
        }
    }
</script>





