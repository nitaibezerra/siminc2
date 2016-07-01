<?php
/*
if ($_POST[pliid]):
	$sql = "DELETE FROM pde.planointernoatividade WHERE atiid = {$_POST[atiid]}";
	$db->executar( $sql );
	unset($sql);
	if (is_array($_POST[pliid]) && array_sum($_POST[pliid])):
		$sql = 'INSERT INTO pde.planointernoatividade (pliid, atiid) VALUES ';
		foreach ($_POST[pliid] as $pliid):
			$sql .= "($pliid, {$_POST[atiid]})";	
		endforeach;
	
		$sql = str_replace(")(", "),(", $sql);
		$db->executar( $sql );
	endif;
	$db->commit();
	die ("<script>
			alert('Operação executada com sucesso!');
			location.href='pde.php?modulo={$_GET[modulo]}&acao={$_GET[acao]}&atiid={$_POST[atiid]}';
		  </script>");
endif;
*/

/*
$atividade = (array) atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_/arvore', 'A' );
}


// VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado( $atividade['atiid'] );
*/
// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba($abacod_tela,$url,'');
monta_titulo($titulo_modulo,'');

$var = $_REQUEST['visualiza'] == '' ? 'PI' : $_REQUEST['visualiza'];
$suprimir = $_REQUEST['suprimir'] == '0' ? 'plano_interno' : '';

?>

<script type="text/javascript">
	function exibirRelatorio()
	{
		var formulario = document.formulario;
		
		formulario.action='monitora.php?modulo=principal/acao/planoInterno&acao=A';
		formulario.submit();
	}

function pesqPlanoInterno () {
	janela = window.open('pde.php?modulo=principal/atividade_/pesqPlanoInterno&acao=A&atiid=<?php echo $_GET[atiid]; ?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-120)+',height=680' ); janela.focus();
}

function directAcao (acaid){
	
}

</script>

<form method="post" name="filtro">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
   <?@$db -> cabecalho_acao($_SESSION['acaid']);?>
		<?php
			$sql = sprintf(
				"select rofmes, max(rofdata) as dataatu, sum(rofdot_ini) as rofdot_ini, sum(rofautorizado) as rofautorizado,  sum(rofempenhado) as empenhado,
				 sum(rofliquidado_favorecido) as rofliquidado_favorecido, sum(rofpago) as rofpago
				 from financeiro.execucao 
				 where prgcod = '%s' and acacod = '%s' and unicod = '%s' and loccod = '%s' and rofano = '%s'
				 group by rofmes, prgcod,acacod, unicod,loccod",
				$prgcod, $acacod, $unicod, $loccod, $_SESSION['exercicio']);

			
			$dadosfinanceiroscompletos = @$db->carregar( $sql );
			$dadofinanceiro = array();
			if($dadosfinanceiroscompletos) {
				foreach($dadosfinanceiroscompletos as $linhafin) {
					//$dataatualizada = $linhafin['dataatu'];
					$dadofinanceiro['rofdot_ini'] += $linhafin['rofdot_ini'];
					$dadofinanceiro['rofautorizado'] += $linhafin['rofautorizado'];
					$dadofinanceiro['empenhado'] += $linhafin['empenhado'];
					$dadofinanceiro['rofliquidado_favorecido'] += $linhafin['rofliquidado_favorecido'];
					$dadofinanceiro['rofpago'] += $linhafin['rofpago'];
					$dadofinanceiropormes[$linhafin['rofmes']] = array("empenhado" => $linhafin['empenhado'],
																	   "liquidado" => $linhafin['rofliquidado_favorecido'],
																	   "pago" 	   => $linhafin['rofpago']);
				}
				$dadofinanceiropormes['totalautorizado'] = $dadofinanceiro['rofautorizado']; 	
			}
//			echo $sql."<br />";
//echo "<pre>";
//			print_r($dadosfinanceiroscompletos);
//echo "</pre>";
		?>
		<tr>
			<td align='right' class="subtitulodireita">Dados Financeiros:</td>
			<td>
				<table class="tabela">
					<tr>
						<td class="SubTituloDireita">Dotação Inicial</td>
						<td class="SubTituloDireita">Dotação ( Lei + Créditos )</td>
						<td class="SubTituloDireita">Empenhado</td>
						<td class="SubTituloDireita">Liquidado</td>
						<td class="SubTituloDireita">Pago</td>
					</tr>
					<tr>
						<?php foreach( $dadofinanceiro as $valor ): ?>
							<td align="right">R$ <?= number_format($valor, 2, ',', '.'); ?></td>
						<?php endforeach; ?>
					</tr>
				</table>
			</td>

		</tr>
		<tr>
			<td align='right' class="subtitulodireita">Visualização:</td>
			<td>
			<form name='formulario' action=''>
				<input type="radio" name="visualiza" value = "PI" <? if ( $var == "PI" ) echo "checked"; ?> onclick="document.filtro.submit();"> Plano Interno &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="radio" name="visualiza" value = "PDE" <? if ( $var == "PDE" ) echo "checked"; ?> onclick="document.filtro.submit();"> Plano de Trabalho &nbsp;
				<input type="checkbox" name="suprimir" value = "1" <? if (isset($_REQUEST['suprimir'])) echo "checked"; ?> onclick="document.filtro.submit();"> Suprimir PI 
			</form>
			</td>

		</tr>
   
</table>
</form>

<?php

/*
$sql = sprintf("SELECT
				 pl.plicod,
                 pl.plidsc as planoInterno
				FROM
				 financeiro.planointerno pl 
				 JOIN monitora.acao ac USING(acaid)
				 JOIN public.unidade un ON un.unicod = ac.unicod 
				WHERE
				 acaid = %d
				ORDER BY
				 plicod",
				$_SESSION[acaid]);			

$cabecalho = array ("Cod. PI", "Descrição PI");
$db->monta_lista($sql, $cabecalho, 100, 20, '', '', '');
*/

if ( $var == 'PI' ) {

/*$sql = "
		SELECT
		 to_char(max(rofdata),'dd/mm/YYYY') as dataatu,
		 pi.plicod || ' ' as codigopi,
		 pi.plidsc,		
		 COALESCE( sum(rofautorizado), 0 ) / 1 AS rofautorizado,
		 COALESCE( sum(rofempenhado), 0 ) / 1 AS empenhado,
		 COALESCE( sum(rofliquidado_favorecido), 0 ) / 1 AS rofliquidado_favorecido,
		 COALESCE( sum(rofpago), 0 ) / 1 as rofpago,
		 CASE WHEN sum( COALESCE( rofautorizado, 0 )) > 0
		  THEN 
		   TRIM(to_char(( sum( COALESCE( rofpago, 0 ) ) * 100 ) / sum( COALESCE( rofautorizado, 0 ) ), '999' ) || ' %')
		  ELSE 		
		   '0 %' 
		 END AS autorizado_porcentagem
		FROM
		 financeiro.planointerno pi 
		 INNER JOIN monitora.acao ac ON pi.acaid = ac.acaid	
		 LEFT JOIN financeiro.execucao ex ON ex.plicod = pi.plicod AND
						     ex.ptres  = pi.pliptres AND
						     ex.acacod = ac.acacod AND 
						     ex.unicod = ac.unicod AND
						     ex.loccod = ac.loccod AND
 						     ex.prgcod = ac.prgcod AND
						     ex.rofano = ac.prgano
		WHERE 
		 ac.acaid = ".$_SESSION['acaid']."	
		GROUP BY
		 pi.plicod,
		 pi.plidsc
union all
SELECT
		 to_char(max(rofdata),'dd/mm/YYYY') as dataatu,
		 ex.plicod || ' ' as codigopi,
		 ex.plidsc,		
		 COALESCE( sum(rofautorizado), 0 ) / 1 AS rofautorizado,
		 COALESCE( sum(rofempenhado), 0 ) / 1 AS empenhado,
		 COALESCE( sum(rofliquidado_favorecido), 0 ) / 1 AS rofliquidado_favorecido,
		 COALESCE( sum(rofpago), 0 ) / 1 as rofpago,
		 CASE WHEN sum( COALESCE( rofautorizado, 0 )) > 0
		  THEN 
		   TRIM(to_char(( sum( COALESCE( rofpago, 0 ) ) * 100 ) / sum( COALESCE( rofautorizado, 0 ) ), '999' ) || ' %')
		  ELSE 		
		   '0 %' 
		 END AS autorizado_porcentagem
		FROM
		 monitora.acao ac 
		 inner JOIN financeiro.execucao ex ON ex.acacod = ac.acacod AND 
						     ex.unicod = ac.unicod AND 
						     ex.loccod = ac.loccod AND
 						     ex.prgcod = ac.prgcod AND 
						     ex.rofano = ac.prgano and trim(ex.plicod) = 'NÃO INFORMADO'
		WHERE 
		 ac.acaid = ".$_SESSION['acaid']." 
		GROUP BY
		 ex.plicod,
		 ex.plidsc ";*/
	
$sql =	"SELECT 
			max(v.rofdata) as dataatu,
			pl.plicod,
			pl.plidsc,
			COALESCE( sum(v.rofautorizado), 0 ) AS rofautorizado,
			COALESCE( sum(v.rofempenhado), 0 ) AS empenhado,
			COALESCE( sum(v.rofliquidado_favorecido), 0 ) AS rofliquidado_favorecido,
			COALESCE( sum(v.rofpago), 0 ) as rofpago,
			CASE WHEN sum( COALESCE( v.rofautorizado, 0 ) ) > 0
			THEN TRIM(to_char(( sum( COALESCE( v.rofpago, 0 ) ) * 100 ) / sum( COALESCE( v.rofautorizado, 0 ) ), '999' ) || ' %')
			ELSE '0 %' 
			END AS autorizado_porcentagem
		 FROM pde.atividade at 
		 INNER JOIN monitora.planointernoatividade p ON p.atiid = at.atiid 
		 INNER JOIN monitora.planointerno pl on pl.pliid = p.pliid
		 INNER JOIN monitora.acao a on a.acaid = pl.acaid
		 INNER JOIN entidade.entidade ent ON ent.entid = at.entid 
		 INNER JOIN public.unidade u on u.unicod = ent.entunicod and u.unitpocod='U' 
		 LEFT JOIN ( SELECT a.atiid, COALESCE( sum(exe.rofautorizado), 0 ) as total 
					 FROM financeiro.execucao exe 
					 INNER JOIN financeiro.planointerno pli on exe.plicod=pli.plicod --and exe.ptres=pli.pliptres 
					 INNER JOIN monitora.planointerno pi ON pi.plicod = pli.plicod
					 INNER JOIN monitora.planointernoatividade a ON a.pliid = pi.pliid
					 GROUP BY a.atiid
					) as z ON z.atiid = at.atiid
		 LEFT JOIN financeiro.execucao v ON v.plicod = pl.plicod AND
		 				     			    v.unicod = ent.entunicod --AND 
						     			   --v.rofano = '2008'
		 WHERE a.acaid = 68807 AND
		 	   at.atistatus = 'A' --AND 
		 	   --at._atinumero >= '17'
		 GROUP BY
			   a.acaid,
			   
		 	   pl.plicod,
		 	   pl.plidsc,
		 	   ent.entunicod,
		 	   ent.entnome,		 	   
		 	   at._atinumero,
		 	   at.atiid,
		 	   z.total
		 ORDER BY _atinumero";
	
	
//				dbg($sql);
	$cabecalho = array(
		'Data <BR>Atualização','Código PI', 'Descrição PI',
		'Lei + Créditos<br>(Autorizado)', 'Empenhado', 'Liquidado', 'Pago', '% do Pago s/<br>Autorizado'
	);
	$db->monta_lista(
		$sql,
		$cabecalho,
		300,
		20,
		'S',
		'',
		''
	);			

}
else
{
?>




<?php
$sql = "SELECT
		 at.atiid,
		 at._atinumero,
		 at._atinumero ||' - '|| at.atidescricao AS atidesc,
		 pl.plicod || ' - ' || pl.plidsc as plano_interno,
		 a.acaid,
		 a.unicod || ' - ' || u.unidsc as unidsc,
		 a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod || ' - ' || a.acadsc || ' - ' || a.sacdsc as acao, 
		 max(v.rofdata) as dataatu,
		 COALESCE( sum(v.rofdotori), 0 ) AS rofdotori,
		 COALESCE( sum(v.rofautorizado), 0 ) AS rofautorizado,
		 COALESCE( sum(v.rofempenhado), 0 ) AS empenhado,
		 COALESCE( sum(v.rofliquidado_favorecido), 0 ) AS rofliquidado_favorecido,
		 COALESCE( sum(v.rofpago), 0 ) as rofpago,
		 CASE WHEN sum( COALESCE( v.rofautorizado, 0 ) ) > 0
		  THEN 
		   TRIM(to_char(( sum( COALESCE( v.rofpago, 0 ) ) * 100 ) / sum( COALESCE( v.rofautorizado, 0 ) ), '999' ) || ' %')
		  ELSE 		
		   '0 %' 
		 END AS autorizado_porcentagem,
		COALESCE( z.total, 0) as totalatividade
		FROM 
		 pde.atividade at 
		 INNER JOIN pde.planointernoatividade p ON p.atiid = at.atiid 
		 inner JOIN financeiro.planointerno pl on pl.pliid = p.pliid 
		 inner join monitora.acao a on a.acaid = pl.acaid 
		 inner JOIN public.unidade u on u.unicod = a.unicod and u.unitpocod='U' 
		 inner join  ( select a.atiid, COALESCE( sum(exe.rofautorizado), 0 ) as total
						from financeiro.execucao exe 
					 inner JOIN financeiro.planointerno pli on exe.plicod=pli.plicod and exe.ptres=pli.pliptres
					 INNER JOIN pde.planointernoatividade a ON a.pliid = pli.pliid
					 group by a.atiid
					 ) as z on z.atiid = at.atiid
		left join financeiro.execucao v on v.plicod=pl.plicod and v.ptres=pl.pliptres 
		WHERE
		a.acaid = ".$_SESSION['acaid']."  and
		 at.atistatus = 'A' AND
		 at._atiprojeto = 3 
		GROUP BY
		 p.pliid,
		 a.acaid, 
		 a.prgcod,
		 a.acacod,
		 a.sacdsc,
		 a.unicod,
		 a.loccod,
		 a.acadsc,
		 pl.plicod,
		 pl.plidsc,
		 u.unidsc,
		 atidesc,
		 at._atinumero,
		 at.atiid,
		 z.total


union all

SELECT
		 9999999 as atiid,
		 '9.9.9.9.9.9' as _atinumero,
		 'Não vinculado' AS atidesc,
		 pl.plicod || ' - ' || pl.plidsc as plano_interno,
		 a.acaid,
		 a.unicod || ' - ' || u.unidsc as unidsc,
		 a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod || ' - ' || a.acadsc || ' - ' || a.sacdsc as acao, 
		 max(v.rofdata) as dataatu,
		 COALESCE( sum(v.rofdotori), 0 ) AS rofdotori,
		 COALESCE( sum(v.rofautorizado), 0 ) AS rofautorizado,
		 COALESCE( sum(v.rofempenhado), 0 ) AS empenhado,
		 COALESCE( sum(v.rofliquidado_favorecido), 0 ) AS rofliquidado_favorecido,
		 COALESCE( sum(v.rofpago), 0 ) as rofpago,
		 CASE WHEN sum( COALESCE( v.rofautorizado, 0 ) ) > 0
		  THEN 
		   TRIM(to_char(( sum( COALESCE( v.rofpago, 0 ) ) * 100 ) / sum( COALESCE( v.rofautorizado, 0 ) ), '999' ) || ' %')
		  ELSE 		
		   '0 %' 
		 END AS autorizado_porcentagem,
		 0 as totalatividade
		FROM
		( select acaid, plicod, plidsc , max(pliptres) as ptres
				from financeiro.planointerno 
				where acaid = ".$_SESSION['acaid']."
				group by acaid, plicod, plidsc ) p 
		 JOIN financeiro.planointerno pl ON pl.plicod = p.plicod and pl.plidsc = p.plidsc and pl.pliptres = p.ptres
		 inner join monitora.acao a on a.acaid = p.acaid
	 
		 inner JOIN public.unidade u on u.unicod = a.unicod and u.unitpocod='U'
		-- left join financeiro.execucao v on v.plicod=pl.plicod and v.plidsc = pl.plidsc and v.ptres=pl.pliptres
		left JOIN financeiro.execucao v ON v.plicod = pl.plicod AND
							v.ptres = pl.pliptres AND
							v.acacod = a.acacod AND 
						     v.unicod = a.unicod AND 
						     v.loccod = a.loccod AND
 						     v.prgcod = a.prgcod AND 
						     v.rofano = a.prgano
		WHERE
		 a.acaid = ".$_SESSION['acaid']."  and
		 pl.pliid not in 
				(  select pl.pliid from pde.atividade at
					 INNER JOIN pde.planointernoatividade p ON p.atiid = at.atiid
					 inner JOIN financeiro.planointerno pl on pl.pliid = p.pliid
					 inner join monitora.acao a on a.acaid = pl.acaid 
				   where a.acaid = ".$_SESSION['acaid']."
				)
		 and pl.pliid not in 
				(  select pl.pliid from pde.atividade at 
					 INNER JOIN monitora.planointernoatividade p ON p.atiid = at.atiid 
					 inner JOIN financeiro.planointerno pl on pl.pliid = p.pliid 
					 inner join monitora.acao a on a.acaid = pl.acaid 
				   where a.acaid = ".$_SESSION['acaid']."
				)
		 
		GROUP BY
		 pl.plicod,
		 pl.plidsc,
		 a.acaid, 
		 a.prgcod,
		 a.acacod,
		 a.sacdsc,
		 a.unicod,
		 a.loccod,
		 a.acadsc,
		 u.unidsc

union all



SELECT
		 9999999 as atiid,
		 '9.9.9.9.9.9' as _atinumero,
		 'NÃO INFORMADO' AS atidesc,
		 'NÃO INFORMADO' as plano_interno,
		 a.acaid,
		 a.unicod || ' - ' || u.unidsc as unidsc,
		 a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod || ' - ' || a.acadsc || ' - ' || a.sacdsc as acao, 
		 max(v.rofdata) as dataatu,
		 COALESCE( sum(v.rofdotori), 0 ) AS rofdotori,
		 COALESCE( sum(v.rofautorizado), 0 ) AS rofautorizado,
		 COALESCE( sum(v.rofempenhado), 0 ) AS empenhado,
		 COALESCE( sum(v.rofliquidado_favorecido), 0 ) AS rofliquidado_favorecido,
		 COALESCE( sum(v.rofpago), 0 ) as rofpago,
		 CASE WHEN sum( COALESCE( v.rofautorizado, 0 ) ) > 0
		  THEN 
		   TRIM(to_char(( sum( COALESCE( v.rofpago, 0 ) ) * 100 ) / sum( COALESCE( v.rofautorizado, 0 ) ), '999' ) || ' %')
		  ELSE 		
		   '0 %' 
		 END AS autorizado_porcentagem,
		 0 as totalatividade
		FROM
		 monitora.acao a 
		 inner JOIN public.unidade u on u.unicod = a.unicod and u.unitpocod='U'
		inner JOIN financeiro.execucao v ON v.acacod = a.acacod AND 
						     v.unicod = a.unicod AND 
						     v.loccod = a.loccod AND
 						     v.prgcod = a.prgcod AND 
						     v.rofano = a.prgano
		WHERE
		 a.acaid = ".$_SESSION['acaid']." and v.plicod = 'NÃO INFORMADO' 
		GROUP BY
		 a.acaid, 
		 a.prgcod,
		 a.acacod,
		 a.sacdsc,
		 a.unicod,
		 a.loccod,
		 a.acadsc,
		 u.unidsc
		ORDER BY
		_atinumero,
		plano_interno, acao
		 ";

$sql1 = "
SELECT at.atiid,
		 		at._atinumero,
		 		at._atinumero ||' - '|| at.atidescricao AS atidesc,
		 		pl.plicod || ' - ' || pl.plidsc as plano_interno,
		 		ent.entunicod || ' - ' || ent.entnome as unidsc,
		 		max(v.rofdata) as dataatu,
		 		a.acaid,
		 		a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod || ' - ' || a.acadsc || ' - ' || a.sacdsc as acao,
		 		COALESCE( sum(v.rofdotori), 0 ) AS rofdotori,
		 		COALESCE( sum(v.rofautorizado), 0 ) AS rofautorizado,
		 		COALESCE( sum(v.rofempenhado), 0 ) AS empenhado,
		 		COALESCE( sum(v.rofliquidado_favorecido), 0 ) AS rofliquidado_favorecido,
		 		COALESCE( sum(v.rofpago), 0 ) as rofpago,
		 		CASE WHEN sum( COALESCE( v.rofautorizado, 0 ) ) > 0
		  		THEN TRIM(to_char(( sum( COALESCE( v.rofpago, 0 ) ) * 100 ) / sum( COALESCE( v.rofautorizado, 0 ) ), '999' ) || ' %')
		  		ELSE '0 %' 
		 		END AS autorizado_porcentagem,
		        COALESCE( z.total, 0) as totalatividade
		 FROM pde.atividade at 
		 INNER JOIN monitora.planointernoatividade p ON p.atiid = at.atiid 
		 INNER JOIN monitora.planointerno pl on pl.pliid = p.pliid
		 INNER JOIN monitora.acao a on a.acaid = pl.acaid
		 INNER JOIN entidade.entidade ent ON ent.entid = at.entid 
		 INNER JOIN public.unidade u on u.unicod = ent.entunicod and u.unitpocod='U' 
		 LEFT JOIN ( SELECT a.atiid, COALESCE( sum(exe.rofautorizado), 0 ) as total 
					 FROM financeiro.execucao exe 
					 INNER JOIN financeiro.planointerno pli on exe.plicod=pli.plicod --and exe.ptres=pli.pliptres 
					 INNER JOIN monitora.planointerno pi ON pi.plicod = pli.plicod
					 INNER JOIN monitora.planointernoatividade a ON a.pliid = pi.pliid
					 GROUP BY a.atiid
					) as z ON z.atiid = at.atiid
		 LEFT JOIN financeiro.execucao v ON v.plicod = pl.plicod AND
		 				     			    v.unicod = ent.entunicod --AND 
						     			   --v.rofano = '2008'
		 WHERE a.acaid = {$_SESSION['acaid']} AND
		 	   at.atistatus = 'A' --AND 
		 	   --at._atinumero >= '17'
		 GROUP BY
			   a.acaid,
			   acao,
		 	   pl.plicod,
		 	   pl.plidsc,
		 	   ent.entunicod,
		 	   ent.entnome,
		 	   atidesc,
		 	   at._atinumero,
		 	   at.atiid,
		 	   z.total
		 ORDER BY _atinumero";			

function agrupar( $lista, $agrupadores ) {
	$existeProximo = count( $agrupadores ) > 0; 
	if ( $existeProximo == false )
	{
		return array();
	}
	$campo = array_shift( $agrupadores );
	$novo = array();
	foreach ( $lista as $item ):
	
		$chave = $item[$campo];
		if ( array_key_exists( $chave, $novo ) == false ){			
			$novo[$chave] = array(
				"atiid"		 => '',
				"acaid"		 => '',
				"autorizado" => 0,
				"empenhado"  => 0,
				"liquidado"	 => 0,
				"pago"	  	 => 0,
				"pagoAutor"	 => '',
				"dotOrig"	 => 0,
				"sempenhado" => 0,
				"agrupador"  => $campo,
				"sub_itens"  => array()
			);
		}
		if ($campo == 'plano_interno' or $agrupador == 'atidesc' ):		
	
			$novo[$chave][dotOrig]    = $item[rofdotori];
			$novo[$chave][autorizado] = $item[rofautorizado];
			$novo[$chave][empenhado]  = $item[empenhado];
			$novo[$chave][liquidado]  = $item[rofliquidado_favorecido];
			$novo[$chave][pago] 	  = $item[rofpago];
			$novo[$chave][pagoAutor]  = $item[autorizado_porcentagem];																	
			$novo[$chave][dotAtividade]    = $item[totalatividade];
		endif;
		
		$novo[$chave]['atiid'] = $campo == 'atidesc' ? $item['atiid'] : '';
		$novo[$chave]['acaid'] = $campo == 'acao' 	 ? $item['acaid'] : '';
					
		$novo[$chave][dotAtividade]    = $item[totalatividade];
		$novo[$chave][sempenhado]  += $item[empenhado];
		$novo[$chave][sdotOrig]    += $item[rofdotori];
		$novo[$chave][sautorizado] += $item[rofautorizado];
		$novo[$chave][sliquidado]  += $item[rofliquidado_favorecido];
		$novo[$chave][spago] 	   += $item[rofpago];
		$novo[$chave][sdotAtividade] 	   += $item[totalatividade];
		
		if ( $existeProximo )
			array_push( $novo[$chave]["sub_itens"], $item );

	endforeach;
	if ( $existeProximo ):
		foreach ( $novo as $chave => $dados )
			$novo[$chave]["sub_itens"] = agrupar( $novo[$chave]["sub_itens"], $agrupadores );		
	endif;
	return $novo;
}
			
			
function exibir( $lista, $profundidade = 0 ){
	global $agrupadorr,$totAutorizado,$totEmpenhado,
		   $totLiquidado,$totPago,$totDotacao,
		   $subAutorizado,$subEmpenhado,$subLiquidado,
		   $subPago,$subDotacao;	
	//static $subAutorizado=0,$subEmpenhado=0,$subLiquidado=0,$subPago=0,$subDotacao=0;	   
	
	if ( count( $lista ) == 0 )
	{		
		return;
	}	
	$contCor = 0;
	foreach ( $lista as $chave => $dados ):
		$autorizado 	 = (string) $dados['autorizado'];	
		$empenhado  	 = (string) $dados['empenhado'];	
		$liquidado  	 = (string) $dados['liquidado'];	
		$pago 			 = (string) $dados['pago'];	
		$pagoAutor  	 = (string) $dados['pagoAutor'];	
		$agrupador  	 = (string) $dados["agrupador"];
		$dotacaoOriginal = (string) $dados["dotOrig"];
		$atiid			 = (int)	$dados["atiid"];
		$acaid			 = (int)	$dados['acaid'];
		$dotAtividade 	 = (string) $dados['dotAtividade'];	

					

		$sautorizado 	  = (string) $dados['sautorizado'];	
		$sempenhado  	  = (string) $dados['sempenhado'];	
		$sliquidado  	  = (string) $dados['sliquidado'];	
		$spago 			  = (string) $dados['spago'];	
		$spagoAutor  	  = (string) $dados['spagoAutor'];	
		$sdotacaoOriginal = (string) $dados["sdotOrig"];			
		$sdotAtividade 	  = (string) $dados["sdotAtividade"];	
		
	/*	
		 if (($contCor != 0 && $agrupador == 'atidesc')):

		 	echo '<tr style="background: #DFDFDF;">
		 			<td><b>Totais Atividade:</b></td>
					<td align="right" style="color: rgb(0, 102, 204); backgroud-color:#ccc;">'.number_format($subDotacao,2,',','.').'</td>
					<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subAutorizado,2,',','.').'</td>
					<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subEmpenhado,2,',','.').'</td>	
					<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subLiquidado,2,',','.').'</td>
					<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subPago,2,',','.').'</td>	
					<td align="right">'.number_format(($subPago * 100) / ($subAutorizado ? $subAutorizado : 1),2,'.',',').'%</td>
		 		  </tr>';
		 	
		 	$subAutorizado = 0;
			$subEmpenhado  = 0;
			$subLiquidado  = 0;
			$subPago 	   = 0;
			$subDotacao	   = 0;
	 	 endif;			
	*/	
		if ($profundidade == 0)
			echo '<tr bgcolor="#DDDDDD" onmouseout="this.bgColor=\'#DDDDDD\';" onmouseover="this.bgColor=\'#ffffcc\';">';
		elseif ($profundidade == 1)
			echo '<tr bgcolor="#E9E9E9" onmouseout="this.bgColor=\'#E9E9E9\';" onmouseover="this.bgColor=\'#ffffcc\';">'; 
		elseif ($profundidade == 2)
			echo '<tr bgcolor="#f1f1f1" onmouseout="this.bgColor=\'#f1f1f1\';" onmouseover="this.bgColor=\'#ffffcc\';">'; 		
		else
			echo '<tr bgcolor="#f9f9f9" onmouseout="this.bgColor=\'#f9f9f9\';" onmouseover="this.bgColor=\'#ffffcc\';">'; 		
			
		$colspan = $agrupador != 'plano_interno' ? 'colspan="10"' : '';	
		
		$size  = $agrupador == 'unidsc' || $agrupador == 'atidesc' ? '12' : '10';
		
		if ($profundidade == 0)
			$cl = '#000088';
		elseif ($profundidade == 1) 
			$cl = 'rgb(0, 102, 204)';
		elseif ($profundidade == 2) 
			$cl = 'green'; 		
		else
			$cl = '#666666';		
		
		//$color = $agrupador == 'plano_interno' ? '#666666' : '#333333';
		//$color = $agrupador == 'unidsc' ? '#000000' : $color;
?>
			<td <?//=$colspan ?> style="color:<?=$cl;?>;font-size:<?=$size ?>px; padding-left:<?= $profundidade * 20 ?>px;">
				<?php if ( $profundidade > 0 ): ?><img src="../imagens/seta_filho.gif" align="absmiddle"/><?php else: echo '&nbsp;'; endif; ?>
				<?
					if ($profundidade == 0 && $atiid <> '9999999'){
						echo "<b><a href='/pde/pde.php?modulo=principal/atividade_/planoInterno&acao=A&atiid={$atiid}' target='new_{$atiid}' title='Visualizar esta Atividade' style=\"color:{$cl};\">{$chave}</a></b>";
					}elseif ($profundidade == 0 && $atiid == '9999999'){
						echo "<b><span style=\"color:{$cl};\">{$chave}</span></b>";
					}elseif ($profundidade == 2){
						echo "<b><span style=\"color:{$cl};\">{$chave}</span></b>";
					}else{
						echo $chave;
					}	
				?>	
			</td>
<?php 
	$subAutorizado += $autorizado;
	$subEmpenhado  += $empenhado;
	$subLiquidado  += $liquidado;
	$subPago 	   += $pago;
	$subDotacao	   += $dotacaoOriginal;
	$subAtividade  += $dotAtividade;

	if ($agrupador == 'plano_interno' ): 
		$totAutorizado += $autorizado;
		$totEmpenhado  += $empenhado;
		$totLiquidado  += $liquidado;
		$totPago 	   += $pago;
		$totDotacao	   += $dotacaoOriginal;	
		$totAtividade  += $sdotAtividade;	
	endif;

	if ( isset($_REQUEST['suprimir']) ): 
		$totAutorizado += $sautorizado;
		$totEmpenhado  += $sempenhado;
		$totLiquidado  += $sliquidado;
		$totPago 	   += $spago;
		$totDotacao	   += $dotacaoOriginal;	
		$totAtividade  += $sdotAtividade;	
	endif;


	$teste = 0;
	if ( $dotAtividade > 0 ) $teste = ($sautorizado * 100) / ($dotAtividade  ? $dotAtividade  : 1); 

?> 
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'plano_interno' ? $dotacaoOriginal : $sdotacaoOriginal ),2,',','.');?></td> 
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'plano_interno' ? $autorizado : $sautorizado ),2,',','.');?></td> 
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'plano_interno' ? $empenhado  : $sempenhado ),2,',','.'); ?></td> 
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'plano_interno' ? $liquidado  : $sliquidado ),2,',','.'); ?></td> 
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'plano_interno' ? $pago		 : $spago ),2,',','.')?></td>	
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'plano_interno' ? $pagoAutor : ($spago * 100) / ($sautorizado ? $sautorizado : 1) ),2,',','.')?></td> 
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'plano_interno' && $dotAtividade > '0' && $autorizado > '0' ? ($autorizado * 100) / ($dotAtividade  ? $dotAtividade  : 1) : $teste  ),2,',','.')?></td>	
		</tr>
<?		
//	endif;
			exibir( $dados["sub_itens"], $profundidade + 1 );
		$contCor++;	
	endforeach;
}

$dados = $db->carregar($sql);

echo '<table width="95%" cellspacing="0" cellpadding="0" align="center" class="tabela"><TR style="background:#FFF;"><TD colspan="10"></TD></TR>';
echo '<table width="95%" cellspacing="0" cellpadding="0" align="center" class="tabela">';		
	echo '<TR style="background:#FFFFFF;">
			<TD colspan="10" align="center" style="font-weight:bold; font-size:9px; border-top:2px solid black; border-bottom:2px solid black;"><div style="float:center; font-size:13px;" >Plano de Trabalho do PDE</div></TD>
		  </tr>';
if (!$dados){
?>
	<TR style="background:#FFF; color:red;">
		<TD colspan="10" align="center">Não existem PI´s associados ao Plano de Trabalho
		</TD>
	</TR>
</table>
<?
}else{
	# Monta data da ultima atualização
	if (is_array($dados)) {	
		foreach ($dados as $date){
			if ($date['dataatu']){
				$datArr = explode("-",$dados[0][dataatu]);
				$data   = "{$datArr[2]}-{$datArr[1]}-{$datArr[0]}"; 
				break;
			}	
		}		
		unset($datArr);
	}
	$total  = count($dados);
	
	$dados = is_array($dados) ? $dados : array();
	
	if (isset($_REQUEST['suprimir']))
		$agp = array("atidesc" ); 
	else
		$agp = array("atidesc" , "plano_interno" ); 
	$dados = agrupar( $dados, $agp );

	echo '<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">';
	echo '	<TR style="background:#DFDFDF;">';
	echo '		<TD valign="top" style="font-weight:bold;"><div>Atividade</div><div style="padding-left:40px;"><img src="../imagens/seta_filho.gif" align="absmiddle"/> Plano Interno</div></TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Dotação Original</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Lei + Créditos (Autorizado)</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Empenhado</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Liquidado</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Pago</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">% do Pago s/<BR> Autorizado</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">% da Atividade</TD>';
	echo '	</TR>';
	exibir($dados);
	
	$tot = $totPago > 0 && $totAutorizado > 0 ? number_format(( $totPago / $totAutorizado * 100),2,',','') : '0';
	$totA = $totAtividade > 0 && $totAutorizado > 0 ? number_format(( $totAtividade / $totAutorizado * 100),2,',','') : '0';

/*	
	echo '<tr style="background: #DFDFDF;">
	 			<td><b>Totais Atividade:</b></td>
				<td align="right" style="color: rgb(0, 102, 204); backgroud-color:#ccc;">'.number_format($subDotacao,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subAutorizado,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subEmpenhado,2,',','.').'</td>	
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subLiquidado,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subPago,2,',','.').'</td>	
				<td align="right">'.number_format(($subPago * 100) / ($subAutorizado ? $subAutorizado : 1),2,'.',',').'%</td>
	 	  </tr>';
*/			 	
	echo '	<TR style="background:#DFDFDF;">
				<TD>&nbsp;&nbsp;&nbsp;&nbsp;<b>Totais:</b></TD>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totDotacao,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totAutorizado,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totEmpenhado,2,',','.').'</td>	
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totLiquidado,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totPago,2,',','.').'</td>	
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($tot,2,',','.').'</td>	
				<td align="right">'.$totA.'%</td>
			</TR>';	
	
	$data = $data ? "* Dados financeiros atualizados até: $data" : "";		
		
//	echo '<TR style="background:#FFFFFF;">
//			<TD colspan="10" align="right" style="font-weight:bold; font-size:9px; border-top:2px solid black; border-bottom:2px solid black;"><div style="float:left; font-size:11px;">Total de registros: '.$total.'</div></TD>
//		  </tr>';
//	echo "</table>";
}

	echo '<table width="95%" cellspacing="0" cellpadding="0" align="center" class="tabela">';		
	echo '<TR style="background:#FFFFFF;">
			<TD colspan="10" align="center" style="font-weight:bold; font-size:9px; border-top:2px solid black; border-bottom:2px solid black;"><div style="float:center; font-size:13px;" >Plano de Trabalho do PPA</div></TD>
		  </tr>';
	echo "</table>";
	
$dados = $db->carregar($sql1);

echo '<table width="95%" cellspacing="0" cellpadding="0" align="center" class="tabela"><TR style="background:#FFF;"><TD colspan="10"></TD></TR>';
if (!$dados){
?>
	<TR style="background:#FFF; color:red;">
		<TD colspan="10" align="center">Não existem PI´s associados ao Plano de Trabalho
		</TD>
	</TR>
</table>

<?
}else{
	# Monta data da ultima atualização
	if (is_array($dados)) {	
		foreach ($dados as $date){
			if ($date['dataatu']){
				$datArr = explode("-",$dados[0][dataatu]);
				$data   = "{$datArr[2]}-{$datArr[1]}-{$datArr[0]}"; 
				break;
			}	
		}		
		unset($datArr);
	}
	
	$total  += count($dados);
	
	$dados = is_array($dados) ? $dados : array();
	
	if (isset($_REQUEST['suprimir']))
		$agp = array("atidesc" ); 
	else
		$agp = array("atidesc" , "plano_interno" ); 
	$dados = agrupar( $dados, $agp );

	echo '<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">';
	echo '	<TR style="background:#DFDFDF;">';
	echo '		<TD valign="top" style="font-weight:bold;"><div>Atividade</div><div style="padding-left:40px;"><img src="../imagens/seta_filho.gif" align="absmiddle"/> Plano Interno</div></TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Dotação Original</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Lei + Créditos (Autorizado)</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Empenhado</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Liquidado</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Pago</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">% do Pago s/<BR> Autorizado</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">% da Atividade</TD>';
	echo '	</TR>';
	exibir($dados);
	
	$tot = $totPago > 0 && $totAutorizado > 0 ? number_format(( $totPago / $totAutorizado * 100),2,',','') : '0';
	$totA = $totAtividade > 0 && $totAutorizado > 0 ? number_format(( $totAtividade / $totAutorizado * 100),2,',','') : '0';

/*	
	echo '<tr style="background: #DFDFDF;">
	 			<td><b>Totais Atividade:</b></td>
				<td align="right" style="color: rgb(0, 102, 204); backgroud-color:#ccc;">'.number_format($subDotacao,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subAutorizado,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subEmpenhado,2,',','.').'</td>	
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subLiquidado,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subPago,2,',','.').'</td>	
				<td align="right">'.number_format(($subPago * 100) / ($subAutorizado ? $subAutorizado : 1),2,'.',',').'%</td>
	 	  </tr>';
*/			 	
	echo '	<TR style="background:#DFDFDF;">
				<TD>&nbsp;&nbsp;&nbsp;&nbsp;<b>Totais:</b></TD>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totDotacao,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totAutorizado,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totEmpenhado,2,',','.').'</td>	
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totLiquidado,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totPago,2,',','.').'</td>	
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($tot,2,',','.').'</td>	
				<td align="right">'.$totA.'%</td>
			</TR>';	
	
	$data = $data ? "* Dados financeiros atualizados até: $data" : "";		
		
	echo '<TR style="background:#FFFFFF;">
			<TD colspan="10" align="right" style="font-weight:bold; font-size:9px; border-top:2px solid black; border-bottom:2px solid black;"><div style="float:left; font-size:11px;">Total de registros: '.$total.'</div>'.$data.'</TD>
		  </tr>';
	echo "</table>";
}


}
?>
<!-- 
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr style="background-color: #cccccc">	
		<td>
			<input type="button" name="botao" value="Incluir Plano Interno" onclick="javascript:pesqPlanoInterno();" style="visibility: <?php //echo $visibleButton; ?>"/>
			<input type="hidden" name="atiid" value="<?php //echo $_GET[atiid]; ?>">
		</td>
	</tr>		
</table>
-->	
<?php

$_SESSION['acaid'] =$_SESSION['acaid'];
	?>