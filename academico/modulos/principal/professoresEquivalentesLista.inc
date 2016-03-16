<?php 

function montaSqlProfEquivalente($filtros = array())
{
	$ano = $_POST['ano'];
	$mes = $_POST['mes'];
	$portaria = $_POST['portaria'];
	$entid = $_POST['entid'];
	
	
	
	$arWhere[] = " mpe.entid = $entid ";
	// Filtros academico.portariaprofequival
	//if($_GET['ano']) $arWherePpe[] = "ppe.ppeano = {$_GET['ano']}";
	if( $ano[0] ){
		//$arWherePpe[] = " ppe.ppeano " . (( $ano_campo_excludente == null || $ano_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ano ) . "') ";		
		//$arWhereMpe[] = " mpe.mpeano " . (( $ano_campo_excludente == null || $ano_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ano ) . "') ";
		$arWhere[] = " mpe.mpeano " . (( $ano_campo_excludente == null || $ano_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ano ) . "') ";
	}
	
	//if($_GET['semestre']) $arWherePpe[] = "ppe.ppesemestre = {$_GET['semestre']}";
	if( $mes[0] ){
		//$arWherePpe[] = " ppe.ppesemestre " . (( $mes_campo_excludente == null || $mes_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $mes ) . "') ";		
		//$arWhereMpe[] = " mpe.mpemes " . (( $mes_campo_excludente == null || $mes_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $mes ) . "') ";
		$arWhere[] = " mpe.mpemes " . (( $mes_campo_excludente == null || $mes_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $mes ) . "') ";
	}
	
	//if($_GET['portaria']) $arWherePpe[] = "ppe.ppeid = {$_GET['portaria']}";
	if( $portaria[0] ){
		//$arWherePpe[] = " ppe.ppesemestre " . (( $mes_campo_excludente == null || $mes_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $mes ) . "') ";		
		//$arWhereMpe[] = " mpe.mpemes " . (( $mes_campo_excludente == null || $mes_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $mes ) . "') ";
		$arWhere[] = " mpe.ppeid " . (( $portaria_campo_excludente == null || $portaria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $portaria ) . "') ";
	}
	// Filtros academico.movprofequivalente
	//if($_GET['ano']) $arWhereMpe[] = "mpe.mpeano = {$_GET['ano']}";
	//if($_GET['mes']) $arWhereMpe[] = "mpe.mpemes = {$_GET['mes']}";
	//if($_GET['portaria']) $arWhereMpe[] = "mpe.ppeid = {$_GET['portaria']}";
	
	// Filtros academico.portariavalor
	//if($_GET['portaria']) $arWherePtv[] = "ptv.ppeid = {$_GET['portaria']}";

	// Calculo saldo
	//$stCalculo = "coalesce((coalesce(ptvvalor,0)) - ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) ))";
	
	// Banco Eqv
	//$stBancoEqv = "coalesce( ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) ),0)";
	
	$sql = "SELECT
				mpe.mpeano || ' ' as ano,
				mes.mesdsc as mes,
				ppe.ppenumero || ' ' as portaria,
				mpe.mpevlr20h || ' ' as h20,
				mpe.mpevlr40h || ' ' as h40,
				mpe.mpevlrdedexclusiva || ' ' as exclusiva,			
				mpe.mpevlrsubstituto || ' ' as substituto,
				mpe.mpevlrvisitante || ' ' as visitante,
				case when coalesce((mpevlr20h+mpevlr40h+mpevlrdedexclusiva+mpevlrsubstituto+mpevlrvisitante),0) > 0 then 
					trim(to_char(coalesce((mpevlr20h+mpevlr40h+mpevlrdedexclusiva+mpevlrsubstituto+mpevlrvisitante),0), '999G999G999G999D99')) 
				     else 
					'0,00' 
				end as total,
				case when coalesce( ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) ),0) > 0 then
					trim(to_char(coalesce( ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) ),0), '999G999G999G999D99')) 
				     else 
					'0,00' 
				end as mpevlrbcequiv,
				case when ptvvalor > 0 then 
					trim(to_char(coalesce(ptvvalor,0), '999G999G999G999D99')) 
				     else '0,00' 
				end as ptvvalor,
				case when coalesce((coalesce(ptvvalor,0)) - ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) )) = 0 then
					'0,00' 
				     else 
					trim(to_char(coalesce((coalesce(ptvvalor,0)) - ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) )), '999G999G999G999D99')) 
				end as saldo
			FROM
				academico.movprofequivalente mpe
			LEFT JOIN 
				academico.portariaprofequival ppe ON ppe.ppeid = mpe.ppeid
			LEFT JOIN
				academico.portariavalor ptv ON ptv.entid = mpe.entid and ptv.ppeid = ppe.ppeid
			LEFT JOIN
				public.meses mes on mes.mescod::integer = mpe.mpemes
			WHERE
				mpe.mpestatus = 'A' 
			 ".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')." 
			ORDER BY
				 1,2";	
	/*
	$sql = "SELECT
			UPPER(entsig) as codigo,				
			UPPER(entnome) as orgao, 
			upper(mun.estuf) as uf,
			ppe.ppenumero,
			ppeano,
			mes.mesdsc,
			'<input type=\"text\" name=\"mpevlr20h[' || e.entid || ']\" id=\"mpevlr20h_' || e.entid || '\" value=\"' || case when mpevlr20h > 0 then trim(to_char(coalesce(mpevlr20h,0), '999G999G999G999D99')) else '0,00' end || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlr20h,
			'<input type=\"text\" name=\"mpevlr40h[' || e.entid || ']\" id=\"mpevlr40h_' || e.entid || '\" value=\"' || case when mpevlr40h > 0 then trim(to_char(coalesce(mpevlr40h,0), '999G999G999G999D99')) else '0,00' end || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlr40h,
			'<input type=\"text\" name=\"mpevlrdedexclusiva[' || e.entid || ']\" id=\"mpevlrdedexclusiva_' || e.entid || '\" value=\"' || case when mpevlrdedexclusiva > 0 then trim(to_char(coalesce(mpevlrdedexclusiva,0), '999G999G999G999D99')) else '0,00' end || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrdedexclusiva,			
			
			'<input type=\"text\" name=\"mpevlrsubstituto[' || e.entid || ']\" id=\"mpevlrsubstituto_' || e.entid || '\" value=\"' || case when coalesce(mpevlrsubstituto,0) > 0 then trim(to_char(coalesce(mpevlrsubstituto,0), '999G999G999G999D99')) else '0,00' end || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrsubstituto,
			'<input type=\"text\" name=\"mpevlrvisitante[' || e.entid || ']\" id=\"mpevlrvisitante_' || e.entid || '\" value=\"' || case when coalesce(mpevlrvisitante,0) > 0 then trim(to_char(coalesce(mpevlrvisitante,0), '999G999G999G999D99')) else '0,00' end || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrvisitante,

			'<span id=\"total_' || e.entid || '\">' || case when coalesce((mpevlr20h+mpevlr40h+mpevlrdedexclusiva+mpevlrsubstituto+mpevlrvisitante),0) > 0 then trim(to_char(coalesce((mpevlr20h+mpevlr40h+mpevlrdedexclusiva+mpevlrsubstituto+mpevlrvisitante),0), '999G999G999G999D99')) else '0,00' end || '</span>' as total,
			
			'
			<span id=\"mpevlrbcequiv_' || e.entid || '\">' || case when {$stBancoEqv} > 0 then trim(to_char({$stBancoEqv}, '999G999G999G999D99')) else '0,00' end || '</span>
			<input type=\"hidden\" name=\"mpevlrbcequiv[' || e.entid || ']\" value=\"' || case when {$stBancoEqv} > 0 then trim(to_char({$stBancoEqv}, '999G999G999G999D99')) else '0,00' end || '\" />
			' as mpevlrbcequiv,
			
			'<span id=\"ptvvalor_' || e.entid || '\">' || case when ptvvalor > 0 then trim(to_char(coalesce(ptvvalor,0), '999G999G999G999D99')) else '0,00' end || '</span>' as ptvvalor,
			'
			<span id=\"saldo_' || e.entid || '\">' || case when {$stCalculo} = 0 then '0,00' else trim(to_char({$stCalculo}, '999G999G999G999D99')) end || '</span>
			<input type=\"hidden\" name=\"entid[]\" value=\"' || e.entid || '\" />
			<input type=\"hidden\" name=\"mpevlrsaldo[' || e.entid || ']\" value=\"' || case when {$stCalculo} = 0 then '0,00' else trim(to_char({$stCalculo}, '999G999G999G999D99')) end || '\" />
			' as portmpmec
		FROM
			entidade.entidade e 
		INNER JOIN
			entidade.funcaoentidade ef ON ef.entid = e.entid
		LEFT JOIN
			entidade.endereco ed ON ed.entid = e.entid
		LEFT JOIN
			territorios.municipio mun ON mun.muncod = ed.muncod		
		LEFT JOIN
			academico.portariavalor ptv ON ptv.entid = e.entid
		LEFT JOIN 
			academico.portariaprofequival ppe ON ppe.ppeid = ptv.ppeid
		LEFT JOIN
			academico.movprofequivalente mpe ON mpe.ppeid = ppe.ppeid AND mpestatus = 'A' AND mpe.entid = e.entid
		LEFT JOIN
			public.meses mes on mes.mescod::integer = mpe.mpemes
		WHERE
			e.entstatus = 'A' 
		AND ef.funid  in ('12')
		".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
		$anoWhere
		$mesWhere		
		ORDER BY
			 e.entsig, e.entnome";
	*/
	
	//ver($sql, d);
	return $sql;
}
/*
if($_REQUEST['requisicao'] == 'carregaComboPortarias'){
	
	$ano = $_REQUEST['ano'];
	$semestre = $_REQUEST['mes'] <= 6 ? 1 : 2;
	
	$sql = "select 
				ppeid as codigo, 
				ppenumero as descricao 
			from 
				academico.portariaprofequival 
			where 
				ppeano = {$ano} 
			and 
				ppesemestre = {$semestre}";
	
	$db->monta_combo('portaria', $sql, 'S', 'Selecione...', '', '', '', '', 'S');
	die;
}
*/
/*
if($_REQUEST['requisicao'] == 'salvaDados'){
	
	if(is_array($_POST['entid'])){
		
		$ppeid 	= $_POST['portaria'];
		$mpeano = $_POST['ano'];
		$mpemes = $_POST['mes'];
		$sql 	= "";
		
		if($ppeid && $mpeano && $mpemes){
		
			foreach($_POST['entid'] as $entid){
				
				$mpeid = $db->pegaUm("select mpeid from academico.movprofequivalente where entid = {$entid} and ppeid = {$ppeid} and mpeano = {$mpeano} and mpemes = {$mpemes}");
				
				$mpevlr20h 			= desformata_valor($_POST['mpevlr20h'][$entid]);
				$mpevlr40h 			= desformata_valor($_POST['mpevlr40h'][$entid]);
				$mpevlrdedexclusiva = desformata_valor($_POST['mpevlrdedexclusiva'][$entid]);
				$mpevlrsubstituto 	= desformata_valor($_POST['mpevlrsubstituto'][$entid]);
				$mpevlrvisitante 	= desformata_valor($_POST['mpevlrvisitante'][$entid]);
  				$mpevlrvagos 		= desformata_valor($_POST['mpevlrvagos'][$entid]);
  				$mpevlrbcequiv 		= desformata_valor($_POST['mpevlrbcequiv'][$entid]);
  				$mpevlrsaldo 		= desformata_valor($_POST['mpevlrsaldo'][$entid]);
				
				if($mpeid){
					
					$sql .= "update academico.movprofequivalente set 
								mpevlr20h 			= {$mpevlr20h},
								mpevlr40h 			= {$mpevlr40h},
								mpevlrdedexclusiva 	= {$mpevlrdedexclusiva},
								mpevlrsubstituto 	= {$mpevlrsubstituto},
								mpevlrvisitante 	= {$mpevlrvisitante},
			  					mpevlrvagos 		= {$mpevlrvagos},
			  					mpevlrbcequiv 		= {$mpevlrbcequiv},
			  					mpevlrsaldo 		= {$mpevlrsaldo}
			  				where 
			  					mpeid = {$mpeid}; ";
					
//					ver($sql, d);
				}else{
					
					$sql .= "insert into academico.movprofequivalente
						(
							entid,ppeid,mpeano,mpemes,mpevlr20h,mpevlr40h,mpevlrdedexclusiva,mpevlrsubstituto,mpevlrvisitante,
	  					 	mpevlrvagos,mpevlrbcequiv,mpevlrsaldo,mpedtinclusao,mpestatus
	  					 )
	  				values
	  					(
	  						{$entid}, {$ppeid}, {$mpeano}, {$mpemes}, {$mpevlr20h}, {$mpevlr40h}, {$mpevlrdedexclusiva},
	  						{$mpevlrsubstituto},{$mpevlrvisitante},{$mpevlrvagos},{$mpevlrbcequiv},{$mpevlrsaldo},now(),'A'
	  					); ";		
				}
			}
//			ver($sql, d);
			if(!empty($sql)){
				$db->executar($sql);
				if($db->commit()){
					echo 1;
					die;
				}
			}
		}
	}	
	echo 0;
	die;
}
*/


?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
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

	   if (x == 1) ret = ' - ' + ret;return ret;

	}
	
	function salvarHoras(){
		var formulario = document.formulario;
	
		if (formulario.elements['ano'][0].value == ''){
			alert('Selecione pelo menos um Ano!');
			return false;
		}	
		
		selectAllOptions( formulario.ano );
		selectAllOptions( formulario.mes );
		selectAllOptions( formulario.portaria );
		
		formulario.submit();
	}
	
</script>
<?php 


require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$mpeano = $_REQUEST['ano'] ? $_REQUEST['ano'] : date('Y');
$entid  = $_SESSION['academico']['entid'];
$orgid  = $_SESSION['academico']['orgid'];

$abacod_tela = 57135;
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Professores Equivalentes", "");

// cria o cabeçalho padrão do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
echo $autoriazacaoconcursos->cabecalho_entidade($entid);

$habil = 'S';

?>
<form name="formulario" id="formulario" method="post" action="">
	<input type="hidden" name="entid" value="<?=$entid?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloDireita" width="40%" >Ano(s):</td>
			<td>
				<?php 
				if($_POST['ano'][0]){
					foreach($_POST['ano'] as $v){
						$sqlAno .= " UNION SELECT '".$v."' AS codigo, '".$v."' AS descricao";
					}
					if($sqlAno){
						$sqlAno = $sqlAno . " order by 1";
						$sqlAno = substr($sqlAno,6);
						$ano = $db->carregar($sqlAno);
					}
				}
				
				$sql_combo = "SELECT '2008' AS codigo, '2008' AS descricao
						  UNION
						  SELECT '2009' AS codigo, '2009' AS descricao
						  UNION
						  SELECT '2010' AS codigo, '2010' AS descricao
						  UNION
						  SELECT '2011' AS codigo, '2011' AS descricao
						  UNION
						  SELECT '2012' AS codigo, '2012' AS descricao
						  UNION
						  SELECT '2013' AS codigo, '2013' AS descricao
						  UNION
						  SELECT '2014' AS codigo, '2014' AS descricao
						  UNION
						  SELECT '2015' AS codigo, '2015' AS descricao
						  UNION
						  SELECT '2016' AS codigo, '2016' AS descricao
						  UNION
						  SELECT '2017' AS codigo, '2017' AS descricao
						  UNION
						  SELECT '2018' AS codigo, '2018' AS descricao
						  UNION
						  SELECT '2019' AS codigo, '2019' AS descricao
						  UNION
						  SELECT '2020' AS codigo, '2020' AS descricao
						ORDER BY 1";
				
				combo_popup( 'ano', $sql_combo, 'Selecione a(s) Ano(s)', '360x460', '', '', '', 'S', false, '', '5', '300' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Mês(es):</td>
			<td id="td_mes">
				<?php 
				
				if($_POST['mes'][0]){
					foreach($_POST['mes'] as $v){
						$sqlMes .= " UNION SELECT '".$v."' AS codigo, '".mes_extenso($v)."' AS descricao";
					}
					if($sqlMes){
						$sqlMes = $sqlMes . " order by 1";
						$sqlMes = substr($sqlMes,6);
						$mes = $db->carregar($sqlMes);
					}
				}
				
				
				$sql_combo = "SELECT '1' AS codigo, 'JANEIRO' AS descricao
							  UNION
							  SELECT '2' AS codigo, 'FEVEREIRO' AS descricao
							  UNION
							  SELECT '3' AS codigo, 'MARÇO' AS descricao
							  UNION
							  SELECT '4' AS codigo, 'ABRIL' AS descricao
							  UNION
							  SELECT '5' AS codigo, 'MAIO' AS descricao
							  UNION
							  SELECT '6' AS codigo, 'JUNHO' AS descricao
							  UNION
							  SELECT '7' AS codigo, 'JULHO' AS descricao
							  UNION
							  SELECT '8' AS codigo, 'AGOSTO' AS descricao
							  UNION
							  SELECT '9' AS codigo, 'SETEMBRO' AS descricao
							  UNION
							  SELECT '10' AS codigo, 'OUTUBRO' AS descricao
							  UNION
							  SELECT '11' AS codigo, 'NOVEMBRO' AS descricao
							  UNION
							  SELECT '12' AS codigo, 'DEZEMBRO' AS descricao
							ORDER BY 1";
															
				combo_popup( 'mes', $sql_combo, 'Selecione a(s) Mês(es)', '360x460', '', '', '', 'S', false, '', '5', '300' );
				?>
			</td>			
		</tr>
		<tr>
			<td class="subtituloDireita">Portaria(s):</td>
			<td>
				<?
				if($_POST['portaria'][0]){
					$sql = "select 
								ppeid as codigo, 
								ppenumero as descricao 
							from 
								academico.portariaprofequival 
							where ppeid in (".implode(",",$_POST['portaria']).")
							ORDER BY 2";
					$portaria = $db->carregar($sql);
				}
				
				$sql_combo = "select 
							ppeid as codigo, 
							ppenumero as descricao 
						from 
							academico.portariaprofequival 
						ORDER BY 2";
				
				combo_popup( 'portaria', $sql_combo, 'Selecione a(s) Portaria(s)', '360x460', '', '', '', 'S', false, '', '5', '300' );
				?>
			</td>
		</tr>
		<tr>
			<td bgcolor="silver" colspan="2">
				<center>
					<p>
						<input type="button" value="Pesquisar" onclick="salvarHoras()">
					</p>
				</center>
			</td>
		</tr>
	</table>
</form>
<div id="divListaUniversidades">
	<?php
	if($_POST){
		$sql = montaSqlProfEquivalente($_POST);
		
		$cabecalho = array('Ano', 'Mês', 'Nº Portaria', '20h', '40h', 'DE', 'Subst.', 'Visit.', 'Total', 'BancoPEqv(ocupado)', 'Portaria MEC BPEQ', 'Saldo');
		$db->monta_lista($sql, $cabecalho, 1000000, 10, 'N', '', '', 'formValores', '');
	}	
	?>
</div>

