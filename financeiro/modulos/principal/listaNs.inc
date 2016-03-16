<?
// Parâmetros para a nova conexão com o banco do SIAFI
$servidor_bd = $servidor_bd_siafi;
$porta_bd    = $porta_bd_siafi;
$nome_bd     = $nome_bd_siafi;
$usuario_db  = $usuario_db_siafi;
$senha_bd    = $senha_bd_siafi;

$db2 = new cls_banco();

// Parâmetros da nova conexão com o banco do SIAFI para o componente 'combo_popup'.
$dados_conexao = array(
					'servidor_bd' => $servidor_bd_siafi,
					'porta_bd' => $porta_bd_siafi,
					'nome_bd' => $nome_bd_siafi,
					'usuario_db' => $usuario_db_siafi,
					'senha_bd' => $senha_bd_siafi
				);
				

//ano do exercicio				
$ano = $_REQUEST['ano'] ? $_REQUEST['ano'] : $_SESSION['exercicio'];
$ano = "2010";


if($_REQUEST['op5'] == 'download'){
	
	// Inclui componente de relatórios
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	$sql   = monta_sql($_REQUEST['lnsid']);
	$dados = $db2->carregar($sql);
	$agrup = monta_agp();
	$col   = monta_coluna();
	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $dados); 
	$r->setColuna($col);
	$r->setTotNivel(true);
	
	$nomeDoArquivoXls = "SIMEC_LISTANS_Relat".date("YmdHis");
	echo $r->getRelatorioXls();
	//exit;
}



if($_REQUEST['op5'] == 'delete'){
	
	$sql = "UPDATE financeiro.listans SET lnsstatus='I' WHERE lnsid = ".$_REQUEST['lnsid'];
	$db2->executar($sql);
	$db2->commit();
	?>
 	<script>
	 		alert("Operação Realizada com Sucesso!");
			location.href='financeiro.php?modulo=principal/listaNs&acao=A';
	</script>
	<? 
	exit;
}


if ( $_POST['nid'] ){
	
	$flagTransacao = false;
	
	$sql = "INSERT INTO financeiro.listans(lnsdsc, usucpf, lnsstatus)
    		VALUES ('".$_POST['nomearquivo']."','".$_SESSION['usucpf']."', 'A') RETURNING lnsid;";
	$lnsid = $db2->pegaUm($sql);
	 
	foreach($_POST['nid'] as $n){
		
		if($_POST['cmb_'.$n]){
			
			list($ns, $nob, $ug, $credor) = explode("_", $n);

			
			$sql = "select distinct  
						ns.numero_ns as ns, 
						ob.numero_ob as ob,
						to_char(ob.data_transacao, 'YYYY/MM/DD') || ' ' || to_char(ob.hora_transacao, 'HH24:MI') as datatransacao,
						ob.observacao as obsob,
						u.ungcod || ' - ' || u.ungdsc as unidade,
						substr(ob.classificacao1_01,2,1) || '.' || substr(ob.classificacao1_01,3,1) || '.' || substr(ob.classificacao1_01,4,2) 
						|| '.' || substr(ob.classificacao1_01,6,2) || '.' || substr(ob.classificacao1_01,8,2) || ' - ' || n.ndpdsc as natureza,
						c.it_co_credor ||' - '|| c.it_no_credor as favorecido,
						ns.valor_transacao_01 as valorns,
						ob.valor_transacao_01 as valorob
						from siafi$ano.ne ne
						  inner join siafi$ano.ns ns ON trim(codigo_inscricao1_01) = substr(ne.numero_ne,12,12) 
						  inner join siafi$ano.ob ob ON trim(gr_an_nu_documento_referencia) = ns.titulo_credito
						  inner join ( select c.it_co_credor, c2.it_no_credor
								from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao
								from siafi$ano.credor c
								where length(c.it_co_credor) = 14 
								group by c.it_co_credor) c
								inner join siafi$ano.credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao ) c 
								ON c.it_co_credor = ob.codigo_favorecido
						  left join dw.naturezadespesa n ON n.ndpcod = substr(ob.classificacao1_01,2,8) 
						  left join dw.ug u ON u.ungcod = ne.codigo_ug_operador 
						where ns.numero_ns = '$ns' 
						and ob.numero_ob = '$nob'
						and u.ungcod = '$ug'
						and c.it_co_credor = '$credor' ";	

			$dados = $db2->pegaLinha($sql);
			
			if($dados){
				$sql = "INSERT INTO financeiro.listansdetalhes(lnsid, lndmesano, ns, nob, dtob, obsob, 
						ug, natureza, favorecido, vlbruto, vlliquido)
	    				VALUES ($lnsid, 
	    						'".$_POST['cmb_'.$n]."',
	    						'".$dados['ns']."',
	    						'".$dados['ob']."', 
	    						'".$dados['datatransacao']."', 
	    						'".$dados['obsob']."', 
	    						'".$dados['unidade']."', 
	    						'".$dados['natureza']."', 
	    						'".$dados['favorecido']."',
	    						".$dados['valorns'].",
	    						".$dados['valorob'].")";

				$db2->executar($sql);
				
				$flagTransacao = true;
			}
			
		}
		
	}
	
	
	if($flagTransacao == true){
		$db2->commit();
	}
	else{
		$db2->rollback();	
	}
	
	?>
 	<script>
	 		alert("Operação Realizada com Sucesso!");
			location.href='financeiro.php?modulo=principal/listaNs&acao=A';
	</script>
	<? 
	exit;
	
}

/*
//Ajax
if ($_REQUEST['nsAjax']) {

	$where    = array();
	
	$where[] = "ns.numero_ns in ('".$_REQUEST['nsAjax']."')";
	
	if ( $_SESSION['ugaux'] ){
		$ugx = str_replace("|","','",$_SESSION['ugaux']);
		$where[] = "ne.codigo_ug_operador in ('".$ugx."')";
		$where[] = "ns.codigo_ug_operador in ('".$ugx."')";
		$where[] = "ob.codigo_ug_operador in ('".$ugx."')";
	}
	if ( $_SESSION['naturezaaux'] ){
		$naturezax = str_replace("|","','",$_SESSION['naturezaaux']);
		$where[] = "ne.natureza_despesa in ('".$naturezax."')";
	}
	if ( $_SESSION['favorecidoaux'] ){
		$favorecidox = str_replace("|","','",$_SESSION['favorecidoaux']);
		$where[] = "ne.codigo_favorecido in ('".$favorecidox."')";
	}
		
	$campos = "select distinct  
						 
						numero_ob as ob,
						ns.valor_transacao_01 as valorns,
						ob.valor_transacao_01 as valorOB";
		
		$sql = sprintf("$campos
						 
						from siafi$ano.ne ne
						  inner join siafi$ano.ns ns ON trim(codigo_inscricao1_01) = substr(ne.numero_ne,12,12) 
						  inner join siafi$ano.ob ob ON trim(gr_an_nu_documento_referencia) = ns.titulo_credito
						  inner join siafi$ano.credor ON it_co_credor = ne.codigo_favorecido
						where 
							%s 
						order by 1",
						($where ? implode(' AND ', $where) : ' ') );
		
		//dbg($sql,1);
		$cabecalho = array("OB", "<div align=right>Valor Bruto</div>", "<div align=right>Valor Líquido</div>");
		$db2->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '', '');
	
	exit;
}
*/			
				

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Lista NS', 'Notas de sistemas' );



//trata combo_popup para paginação e ordenação por coluna

		//campo ug
		if(!$_POST['numero'] && !$_POST['ordemlista']){
			if($_POST['ug'][0]){
				for($i=0; $i<count($_POST['ug']);  $i++){
					$ugx .= "|" . $_POST['ug'][$i];
				}
				$_SESSION['ugaux'] = substr($ugx, 1);	
			}
			else{
				$_SESSION['ugaux'] = "";
			}
		}
		if($_SESSION['ugaux']){
			$ugx = str_replace("|","','",$_SESSION['ugaux']);
			//parametro para query
			$sqlx = "SELECT
									 		u.ungcod as codigo,
											u.ungcod || ' - ' || u.ungdsc as descricao 
									   FROM
									   		dw.ug u
									   WHERE
									   		u.ungcod in ('".$ugx."')";

			$_POST['ug'] = $db2->carregarColuna($sqlx);
			//preenche combopopup
			$ug = $db2->carregar($sqlx);
		}

		//campo natureza
		if(!$_POST['numero'] && !$_POST['ordemlista']){
			if($_POST['natureza'][0]){
				for($i=0; $i<count($_POST['natureza']);  $i++){
					$naturezax .= "|" . $_POST['natureza'][$i];
				}
				$_SESSION['naturezaaux'] = substr($naturezax, 1);	
			}
			else{
				$_SESSION['naturezaaux'] = "";
			}
		}
		if($_SESSION['naturezaaux']){
			$naturezax = str_replace("|","','",$_SESSION['naturezaaux']);
			//parametro para query
			$sqlx = "SELECT
									 		cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod || sbecod AS codigo,
											ctecod || '.' || gndcod || '.' || mapcod || '.' || edpcod || '.' || sbecod || ' - ' || ndpdsc AS descricao 
									   FROM
									   		dw.naturezadespesa
									   WHERE
									   		cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod || sbecod in ('".$naturezax."')";

			$_POST['natureza'] = $db2->carregarColuna($sqlx);
			//preenche combopopup
			$natureza = $db2->carregar($sqlx);
		}

		//campo favorecido
		if(!$_POST['numero'] && !$_POST['ordemlista']){
			if($_POST['favorecido'][0]){
				for($i=0; $i<count($_POST['favorecido']);  $i++){
					$favorecidox .= "|" . $_POST['favorecido'][$i];
				}
				$_SESSION['favorecidoaux'] = substr($favorecidox, 1);	
			}
			else{
				$_SESSION['favorecidoaux'] = "";
			}
		}
		if($_SESSION['favorecidoaux']){
			$favorecidox = str_replace("|","','",$_SESSION['favorecidoaux']);
			//parametro para query
			$sqlx = "select distinct c.it_co_credor as codigo, c.it_co_credor ||' - '|| c2.it_no_credor as descricao
								from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao
								from siafi$ano.credor c
								where length(c.it_co_credor) = 14 
								group by c.it_co_credor) c
								inner join siafi$ano.credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao
									   WHERE
									   		c.it_co_credor in ('".$favorecidox."')";

			$_POST['favorecido'] = $db2->carregarColuna($sqlx);
			//preenche combopopup
			$favorecido = $db2->carregar($sqlx);
		}		
//fim trata combo_popup


?>

<script type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<form method="post" action="" name="formulario" id="formulario">

<input type="hidden" name="varaux" value="">

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td width="35%" class="SubTituloDireita" valign="top">Exercício:</td>
		<td>
			<?
				
				$sql = "select prsano as codigo, prsano as descricao,prsexercicioaberto 
							  from ". $_SESSION['sisdiretorio'] .".programacaoexercicio 
							  order by 1";
				$db->monta_combo("ano",$sql,"S","Selecione...","selAno","","","","N","ano","",$ano);
			?>
		</td>
	</tr>

	<tr>
		<td class="SubTituloDireita" valign="top">Unidade Gestora:</td>
		<td>
			<?
				$sql_combo = "SELECT distinct 
							u.ungcod as codigo,
							u.ungcod || ' - ' || u.ungdsc as descricao
							FROM 
							dw.ug u
							WHERE
							u.orgcod ilike '26%'
							ORDER BY
							u.ungcod, u.ungcod || ' - ' || u.ungdsc";
				/*
				if ( $_REQUEST['ug'] && $_REQUEST['ug'][0] != '' )
				{
					$sql_carregados = "SELECT
									 		u.ungcod as codigo,
											u.ungcod || ' - ' || u.ungdsc as descricao 
									   FROM
									   		dw.ug u
									   WHERE
									   		u.ungcod in ('".implode("','", $_REQUEST['ug'])."')";
					$ug = $db2->carregar( $sql_carregados );
				}
				*/
				combo_popup( 'ug', $sql_combo, 'Selecione a(s) Unidade(s) Gestora(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true);
			 
			?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" valign="top">Natureza de Despesa:</td>
		<td>
			<? 
				$sql_combo = "SELECT
								cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod || sbecod AS codigo,
								ctecod || '.' || gndcod || '.' || mapcod || '.' || edpcod || '.' || sbecod || ' - ' || ndpdsc AS descricao 
							  FROM
							  	dw.naturezadespesa
							  ORDER BY
							  	codigo"; 
				/*
				if ( $_REQUEST['natureza'] && $_REQUEST['natureza'][0] != '' )
				{
					$sql_carregados = "SELECT
									 		cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod AS codigo,
											ctecod || '.' || gndcod || '.' || mapcod || '.' || edpcod || ' - ' || ndpdsc AS descricao 
									   FROM
									   		dw.naturezadespesa
									   WHERE
									   		cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod in ('".implode("','", $_REQUEST['natureza'])."')";
					$natureza = $db2->carregar( $sql_carregados );
				}
				*/
				combo_popup( 'natureza', $sql_combo, 'Selecione a(s) Natureza(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
			?>
		</td>
	</tr>

	<tr>
		<td class="SubTituloDireita" valign="top">Favorecido:</td>
		<td>
			<? 
				/*
				$sql_combo = "SELECT distinct
								it_co_credor AS codigo,
								it_co_credor ||' - '|| it_no_mnemonico_credor AS descricao 
							  FROM
							  	siafi$ano.credor
							  where length(trim(it_co_credor)) = 14
							  ORDER BY
							  	descricao";
				*/
				
				$sql_combo = "select distinct c.it_co_credor as codigo, c.it_co_credor ||' - '|| c2.it_no_credor as descricao
								from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao
								from siafi$ano.credor c
								where length(c.it_co_credor) = 14 
								group by c.it_co_credor) c
								inner join siafi$ano.credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao ";
				/*
				if ( $_REQUEST['favorecido'] && $_REQUEST['favorecido'][0] != '' )
				{
					$sql_carregados = "SELECT distinct
									 		it_co_credor AS codigo,
											it_co_credor ||' - '|| it_no_mnemonico_credor AS descricao 
									   FROM
									   		siafi2010.credor
									   WHERE
									   		it_co_credor in ('".implode("','", $_REQUEST['favorecido'])."')";
					$favorecido = $db2->carregar( $sql_carregados );
				}
				*/
				combo_popup( 'favorecido', $sql_combo, 'Selecione o(s) Favorecido(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Nome do Arquivo:</td>
		<td>
			<?= campo_texto( 'nomearquivo', 'N', 'S', '', 60, 60, '', '' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Arquivo(s) Salvo(s):</td>
		<td align="left">
			<?
			$sql = "select '<center><a href=\"financeiro.php?modulo=principal/listaNs&acao=A&lnsid=' || lnsid || '&op5=download\">
								<img border=0 src=\"../imagens/anexo.gif\" title=\"Download do arquivo\" />
							</a>'
							||
							CASE WHEN usucpf = '{$_SESSION['usucpf']}' THEN 
								' <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'financeiro.php?modulo=principal/listaNs&acao=A&lnsid=' || lnsid || '&op5=delete\');\" >
								 	<img border=0 src=\"../imagens/excluir.gif\" />
								 </a>'
							ELSE
								''
							END
							|| '</center>' AS acoes,
							'<center>'||lnsdsc||'</center>' as dsc 
					FROM financeiro.listans 
					WHERE lnsstatus = 'A' and usucpf = '".$_SESSION['usucpf']."'";
			//dbg($sql,1);
			$cabecalho = array("Ação", "Nome do Arquivo");
			echo "<table width='50%'><tr><td>";
			$db2->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '100%');
			echo "</td></tr></table>"
			?>
		</td>
	</tr>
	
	<tr>
		<td bgcolor="#CCCCCC"></td>
		<td bgcolor="#CCCCCC">
			<input type="button" name="consultar" id="consultar" value="Consultar" onclick="submete();" />
			&nbsp;&nbsp;
			<input type="button" name="salvar" id="salvar" value="Salvar XLS" onclick="salvarxls();" />
		</td>
	</tr>
</table>



<?
	$where    = array();
	
	if ( $_SESSION['ugaux'] ){
		$where[] = "ne.codigo_ug_operador in ('".$ugx."')";
		$where[] = "ns.codigo_ug_operador in ('".$ugx."')";
		$where[] = "ob.codigo_ug_operador in ('".$ugx."')";
	}
	if ( $_SESSION['naturezaaux'] ){
		$where[] = "substr(ob.classificacao1_01,2,8) in ('".$naturezax."')";
	}
	if ( $_SESSION['favorecidoaux'] ){
		$where[] = "ob.codigo_favorecido in ('".$favorecidox."')";
	}
	
	if ( $_REQUEST['varaux'] == '1'	){
		
		/*
		$campos = "select distinct  
						--ne.numero_ne, 
						--ne.mes_lancamento, 
						--ne.codigo_favorecido, 
						--ne.valor_transacao,
						--ns.mes_lancamento, 
						--ns.codigo_favorecido, 
						--numero_ob, 
						--ob.codigo_favorecido, 
						--trim(gr_an_nu_documento_referencia), 
						--ns.titulo_credito, 
						
						--'<a href=\"javascript:void(0);\" onclick=\"montaSubLista(\''|| ns.numero_ns ||'\')\"><img id=\"img_mais_'|| ns.numero_ns ||'\" src=\"../imagens/mais.gif\" border=\"0\"></a> <a href=\"javascript:void(0);\" onclick=\"desmontaSubLista(\''|| ns.numero_ns ||'\')\"><img id=\"img_menos_'|| ns.numero_ns ||'\" src=\"../imagens/menos.gif\" border=\"0\" style=\"display:none\"></a> ' || ns.numero_ns as ns,
						ns.numero_ns as ns, 
						numero_ob as ob,
						ns.valor_transacao_01 as valorns,
						ob.valor_transacao_01 as valorOB
						--,'</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=5 style=\"padding-left:20px;\" id=\"td_' || ns.numero_ns || '\" ></td></tr>' as linha
						
						--(ns.valor_transacao_01+ns.valor_transacao_02+ns.valor_transacao_03+ns.valor_transacao_04+ns.valor_transacao_05+ns.valor_transacao_06+ns.valor_transacao_07) as valorns, 
						--(ob.valor_transacao_01+ob.valor_transacao_02+ob.valor_transacao_03+ob.valor_transacao_04+ob.valor_transacao_05) as valorOB";
						
		$sql = sprintf("$campos
						from siafi$ano.ne ne
						  inner join siafi$ano.ns ns ON trim(codigo_inscricao1_01) = substr(ne.numero_ne,12,12) 
						  inner join siafi$ano.ob ob ON trim(gr_an_nu_documento_referencia) = ns.titulo_credito
						  inner join siafi$ano.credor c ON c.it_co_credor = ob.codigo_favorecido
						  left join dw.naturezadespesa n ON n.ndpcod = substr(ob.classificacao1_01,2,8) 
						  left join dw.ug u ON u.ungcod = ne.codigo_ug_operador 
						where 
							%s
							--and substr(ne.numero_ne,7,5) = '15240' 
							--and substr(ns.numero_ns,7,5) = '15240' 
						order by 1",
						($where ? implode(' AND ', $where) : ' ') );	
						
		$cabecalho = array("NS", "OB", "<div align=right>Valor Bruto</div>", "<div align=right>Valor Líquido</div>", "");											
		*/
		
		//$combo = "<select name=\"chk_'|| ns.numero_ns ||'\"><option value=\"\"></option><option value=\"012010\">JAN/2010</option></select>";
		 	
		$campos = "select distinct  
						'<input type=\"hidden\" name=\"nid[]\" value=\"'|| ns.numero_ns || '_' || ob.numero_ob || '_' || u.ungcod || '_' || c.it_co_credor  ||'\">
						<select name=\"cmb_'|| ns.numero_ns || '_' || ob.numero_ob || '_' || u.ungcod || '_' || c.it_co_credor ||'\">
							<option value=\"\"></option>'
							||
							(CASE WHEN ld.lndmesano = '012010' THEN '<option value=\"012010\" selected>JAN/2010</option>' ELSE '<option value=\"012010\">JAN/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '022010' THEN '<option value=\"022010\" selected>FEV/2010</option>' ELSE '<option value=\"022010\">FEV/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '032010' THEN '<option value=\"032010\" selected>MAR/2010</option>' ELSE '<option value=\"032010\">MAR/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '042010' THEN '<option value=\"042010\" selected>ABR/2010</option>' ELSE '<option value=\"042010\">ABR/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '052010' THEN '<option value=\"052010\" selected>MAI/2010</option>' ELSE '<option value=\"052010\">MAI/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '062010' THEN '<option value=\"062010\" selected>JUN/2010</option>' ELSE '<option value=\"062010\">JUN/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '072010' THEN '<option value=\"072010\" selected>JUL/2010</option>' ELSE '<option value=\"072010\">JUL/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '082010' THEN '<option value=\"082010\" selected>AGO/2010</option>' ELSE '<option value=\"082010\">AGO/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '092010' THEN '<option value=\"092010\" selected>SET/2010</option>' ELSE '<option value=\"092010\">SET/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '102010' THEN '<option value=\"102010\" selected>OUT/2010</option>' ELSE '<option value=\"102010\">OUT/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '112010' THEN '<option value=\"112010\" selected>NOV/2010</option>' ELSE '<option value=\"112010\">NOV/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '122010' THEN '<option value=\"122010\" selected>DEZ/2010</option>' ELSE '<option value=\"122010\">DEZ/2010</option>' END)
							||
							(CASE WHEN ld.lndmesano = '012011' THEN '<option value=\"012011\" selected>JAN/2011</option>' ELSE '<option value=\"012011\">JAN/2011</option>' END)
							||
							(CASE WHEN ld.lndmesano = '022011' THEN '<option value=\"022011\" selected>FEV/2011</option>' ELSE '<option value=\"022011\">FEV/2011</option>' END)
							||
							(CASE WHEN ld.lndmesano = '032011' THEN '<option value=\"032011\" selected>MAR/2011</option>' ELSE '<option value=\"032011\">MAR/2011</option>' END)
							||
							(CASE WHEN ld.lndmesano = '042011' THEN '<option value=\"042011\" selected>ABR/2011</option>' ELSE '<option value=\"042011\">ABR/2011</option>' END)
							||	
						'</select>' as acao,
						ns.numero_ns as ns, 
						ob.numero_ob as ob,
						to_char(ob.data_transacao, 'DD/MM/YYYY') || ' ' || to_char(ob.hora_transacao, 'HH24:MI') as datatransacao,
						ob.observacao as obsob,
						u.ungcod || ' - ' || u.ungdsc as unidade,
						substr(ob.classificacao1_01,2,1) || '.' || substr(ob.classificacao1_01,3,1) || '.' || substr(ob.classificacao1_01,4,2) 
						|| '.' || substr(ob.classificacao1_01,6,2) || '.' || substr(ob.classificacao1_01,8,2) || ' - ' || n.ndpdsc as natureza,
						--c.it_co_credor ||' - '|| c.it_no_mnemonico_credor as favorecido,
						c.it_co_credor ||' - '|| c.it_no_credor as favorecido,
						ns.valor_transacao_01 as valorns,
						ob.valor_transacao_01 as valorOB
					";
		 	
		$sql = sprintf("$campos
						 
						from siafi$ano.ne ne
						  inner join siafi$ano.ns ns ON trim(codigo_inscricao1_01) = substr(ne.numero_ne,12,12) 
						  inner join siafi$ano.ob ob ON trim(gr_an_nu_documento_referencia) = ns.titulo_credito
						  inner join ( select c.it_co_credor, c2.it_no_credor
								from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao
								from siafi$ano.credor c
								where length(c.it_co_credor) = 14 
								group by c.it_co_credor) c
								inner join siafi$ano.credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao ) c 
								ON c.it_co_credor = ob.codigo_favorecido
						  left join dw.naturezadespesa n ON n.ndpcod = substr(ob.classificacao1_01,2,8) 
						  left join dw.ug u ON u.ungcod = ne.codigo_ug_operador 
						  left join financeiro.listansdetalhes ld on ld.ns = ns.numero_ns and ld.nob = ob.numero_ob and (ld.ug = u.ungcod || ' - ' || u.ungdsc)
						where 
							%s
						order by 1",
						($where ? implode(' AND ', $where) : ' ') );
		
		//dbg($sql,1);
		$cabecalho = array("Mês/Ano", "NS", "OB", "Data Transação OB", "Observação OB", "Unidade", "Natureza", "Favorecido", "<div align=right>Valor Bruto</div>", "<div align=right>Valor Líquido</div>");
		$db2->monta_lista_simples($sql, $cabecalho, 50000, 10, 'N', '', '');
		
	}
?>


<?php 

function monta_agp(){
	
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"lndid",
											"mesano",
											"ns",
											"ob",
											"dataob",
											"obsob",
											"ug",
											"natureza",
											"favorecido",
											"vlbruto",				
											"vlliquido"			   		
										  )	  
				);

	array_push($agp['agrupador'], array(
						"campo" => "lndid",
						"label" => "Código"));	
				
	/*
	array_push($agp['agrupador'], array(
						"campo" => "mesano",
						"label" => "Mês/Ano"));	

	array_push($agp['agrupador'], array(
						"campo" => "ns",
						"label" => "NS"));	
	
	array_push($agp['agrupador'], array(
						"campo" => "ob",
						"label" => "OB"));	
	
	array_push($agp['agrupador'], array(
						"campo" => "dataob",
						"label" => "Data Transação OB"));	
	
	array_push($agp['agrupador'], array(
						"campo" => "obsob",
						"label" => "Observação OB"));	

	array_push($agp['agrupador'], array(
						"campo" => "ug",
						"label" => "Unidade"));	
	
	array_push($agp['agrupador'], array(
						"campo" => "natureza",
						"label" => "Natureza"));	
	
	array_push($agp['agrupador'], array(
						"campo" => "favorecido",
						"label" => "Favorecido"));	
	
	array_push($agp['agrupador'], array(
						"campo" => "vlbruto",
						"label" => "Valor Bruto"));	
	
	array_push($agp['agrupador'], array(
						"campo" => "vlliquido",
						"label" => "Valor Líquido"));	
	
	*/
				   						 
	return $agp;
}


function monta_coluna(){
		
	$coluna = array();
	

	array_push($coluna, array( "campo" => "mesano",
						       "label" => "Mês/Ano"
										)										
							   		);	

	array_push($coluna, array( "campo" => "ns",
						       "label" => "NS"
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "ob",
						       "label" => "OB"
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "dataob",
						       "label" => "Data Transação OB"
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "obsob",
						       "label" => "Observação OB"
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "ug",
						       "label" => "Unidade"
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "natureza",
						       "label" => "Natureza"
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "favorecido",
						       "label" => "Favorecido"
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "vlbruto",
						       "label" => "Valor Bruto",	
						       "type"  => "decimal"	
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "vlliquido",
						       "label" => "Valor Líquido",	
						       "type"  => "decimal"	
										)										
							   		);	

	

	return $coluna;			  	
}

function monta_sql($lnsid){
	
	
	$sql = "SELECT  lndid, 
			(CASE substr(lndmesano,1,2)
			    WHEN '01' THEN 'JAN/'
			    WHEN '02' THEN 'FEV/'
			    WHEN '03' THEN 'MAR/'
			    WHEN '04' THEN 'ABR/'
			    WHEN '05' THEN 'MAI/'
			    WHEN '06' THEN 'JUN/'
			    WHEN '07' THEN 'JUL/'
			    WHEN '08' THEN 'AGO/'
			    WHEN '09' THEN 'SET/'
			    WHEN '10' THEN 'OUT/'
			    WHEN '11' THEN 'NOV/'
			    WHEN '12' THEN 'DEZ/'
			    ELSE ''
			 END) || substr(lndmesano,3) AS mesano, 
			substr(ns,12) as ns, 
			substr(nob,12) as ob, 
			to_char(dtob, 'DD/MM/YYYY HH24:MI') as dataob,
			obsob, 
			ug, 
			natureza, 
			favorecido, 
			vlbruto, 
			vlliquido
		FROM financeiro.listansdetalhes 
		where lnsid = $lnsid";
			
	return $sql;
}


?>

<script type="text/javascript">

function selAno(ano){
	if(ano){
		document.formulario.submit();
	}
}

function submete() {
	
	selectAllOptions(document.formulario.ug);
	selectAllOptions(document.formulario.natureza);
	selectAllOptions(document.formulario.favorecido);
	document.formulario.varaux.value='1';
	document.formulario.submit();

}


function salvarxls() {
	
	if(document.formulario.nomearquivo.value==''){
		alert("Informe o Nome do Arquivo");
		return false;
	}
	
	document.formulario.varaux.value='0';
	document.formulario.submit();
	
}

function exclusao(url) {
		
	var questao = confirm("Deseja realmente excluir este arquivo?")
	if (questao){
		window.location = url;
	}
}

/*
function montaSubLista(ns)
{
	td 	   = document.getElementById('td_'+ns);
	img_mais   = document.getElementById('img_mais_'+ns);
	img_menos   = document.getElementById('img_menos_'+ns);

	selectAllOptions(document.formulario.ug);
	selectAllOptions(document.formulario.natureza);
	selectAllOptions(document.formulario.favorecido);
	
	// Faz uma requisição ajax, passando o parametro, via POST
	var req = new Ajax.Request('financeiro.php?modulo=principal/listaNs&acao=A', {
			        method:     'post',
			        parameters: '&nsAjax=' + ns,
			        onComplete: function (res)
			        {	
						td.style.display = '';
						//td.style.padding = '5px';
						img_mais.style.display = 'none';
						img_menos.style.display = '';
						td.innerHTML = res.responseText;
			        }
			  });

}



function desmontaSubLista(ns)
{
	td 	   = document.getElementById('td_'+ns);
	img_mais   = document.getElementById('img_mais_'+ns);
	img_menos   = document.getElementById('img_menos_'+ns);
	
	td.innerHTML = '';
	td.style.display = 'none';
	img_mais.style.display = '';
	img_menos.style.display = 'none';

}
*/

</script>