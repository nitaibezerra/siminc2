<?php

cte_verificaSessao();

$inuid = (integer) $_SESSION["inuid"];

if ( isset( $_REQUEST["gravar"] ) )
{
	// captura entidades
	$entids = $_REQUEST["entid"];
	if ( !is_array( $entids ) )
	{
		$entids = array();
	}
	$entids = array_map( "intval", $entids );
	
	// remove quantitativos de escolas não mais utilizadas
	$sql = "
		select
			si.sbaid
		from cte.instrumentounidade iu
			inner join cte.pontuacao po on
				po.inuid = iu.inuid
			inner join cte.acaoindicador ai on
				ai.ptoid = po.ptoid
			inner join cte.subacaoindicador si on
				si.aciid = ai.aciid
		where
			iu.inuid = " . $inuid . " and
			po.ptostatus = 'A'
		group by
			si.sbaid";

	$lista  = $db->carregar( $sql );
	$lista  = $lista ? $lista : array();
	$sbaids = array();

	foreach ( $lista as $item )
	{
		array_push( $sbaids, (integer) $item["sbaid"] );
	}

	if ( count( $entids ) && count( $sbaids ) )
	{
        /*!@
		$sql = "
			delete from cte.qtdfisicoano
			where
				sbaid in ( " . implode( ",", $sbaids ) . " ) and
				entid not in ( " . implode( ",", $entids ) . " )
		";
        //                                                                  */
		//$db->executar( $sql );
	}
	
	// refaz relacionamento escolas -> inuid
	$sql = "
		delete from cte.instrumentounidadeescola
		where
			inuid = " . $inuid . "
	";
	$db->executar( $sql );
	$sqlBase = "
		insert into cte.instrumentounidadeescola
		( inuid, entid )
		values
		( %d, %d )
	";
	foreach ( $entids as $entid )
	{
		$sql = sprintf( $sqlBase, $inuid, $entid );
		$db->executar( $sql );
	}
	$db->commit();
}

$sql = "
	select
		entid
	from cte.instrumentounidadeescola
	where
		inuid = " . $inuid . "
";
$lista = $db->carregar( $sql );
$lista = $lista ? $lista : array();
$entids = array();
foreach ( $lista as $item )
{
	array_push( $entids, $item["entid"] );
}

$muncod = cte_pegarMuncod( $inuid );
$estuf  = cte_pegarEstuf( $inuid );
$condicao = array();

if ($muncod) {
    array_push($condicao, "m.muncod = '" . $muncod . "'" );
} elseif ($estuf) {
    array_push($condicao, "m.estuf = '"  . $estuf . "'" );
}

//array_push( $condicao, "t.funid in ( 3, 4 )" );
$itrid = cte_pegarItrid( $inuid );
if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL )
{

    $sql = "     select distinct
    					e.entid,
                		e.entnome,
                		m.mundescricao
			     from entidade.entidade e
			     inner join entidade.endereco en on e.entid = en.entid and en.tpeid = 1 
			     inner join entidade.funcaoentidade fe ON fe.entid = e.entid
			     inner join territorios.municipio m on m.muncod = en.muncod 
			     where (e.entescolanova = false or e.entescolanova is null) 
			     and e.entnome <> '' 
			     and fe.funid = 3
			     and e.tpcid = 1
			     and  m.estuf = '".$estuf."'
				 group by
                	e.entid,
                	e.entnome,
                	m.mundescricao 
                order by
                	m.mundescricao, 
                	e.entnome    
			";
} else {
    $sql = 'select
                ent.entnome, ent.entid
            from
                entidade.entidade ent
            left join
                entidade.entidadedetalhe entd on ent.entcodent = entd.entcodent
                					and (
					                	 entdreg_infantil_creche = \'1\' or
					                     entdreg_infantil_preescola = \'1\' or
					                     entdreg_fund_8_anos        = \'1\' or
					                     entdreg_fund_9_anos        = \'1\')
            inner join
                entidade.endereco ende on ent.entid = ende.entid
            where
                ent.entescolanova = false
                and ende.muncod = \'' . $muncod . '\'
                and
                ent.tpcid = 3
                
                and
                ent.entstatus = \'A\'
            order by
                ent.entnome';
}
//dbg($sql,1);
$escolas_db = $db->carregar($sql);
$escolas_db = is_array($escolas_db) ? $escolas_db : array();

$qtdEscolas = count( $escolas_db );

$escolas = array();
foreach ( $escolas_db as $escola_item )
{
	$mundescricao = $escola_item["mundescricao"];
	if ( !array_key_exists( $mundescricao, $escolas ) )
	{
		$escolas[$mundescricao] = array();
	}
	array_push( $escolas[$mundescricao], $escola_item );
}

// captura escolas ja utilizadas
$sql = "
	select
		q.entid
	from cte.qtdfisicoano q
		inner join cte.subacaoindicador si on
			si.sbaid = q.sbaid
		inner join cte.acaoindicador ai on
			ai.aciid = si.aciid
		inner join cte.pontuacao po on
			po.ptoid = ai.ptoid
	where
		po.ptostatus = 'A' and
		po.inuid = " . $inuid . "
	group by
		q.entid
";
$dados = $db->carregar( $sql );
$dados = $dados ? $dados : array();
$utilizados = array();
foreach ( $dados as $linha )
{
	array_push( $utilizados, $linha["entid"] );
}

?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title><?= $titulo ?><?= $maximo != 0 ? ' - Ecolha no máximo ' . $maximo . ' itens' : '' ; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript">
		
			function gravarEscolas()
			{
				document.formulario.submit();
			}
			
			function fecharJanela()
			{
				window.opener.location.reload();
				window.close();
			}
			
		</script>
	</head>
	<body style="margin:0;padding:0;">
		<form action="" method="post" name="formulario">
			<input type="hidden" name="gravar" value="1" />
			<table class="tabela" align="center" style="width: 100%" cellspacing="1" cellpadding="3">
				<?php if ( count( $escolas ) ) : ?>
					<tr style="background-color: #dfdfdf; border: 1px solid #cccccc">
						<td colspan="2" style="text-align: center;color: #106010;">
							Pressione Ctrl+F para localizar a escola.
						</td>
					</tr>				
					<tr style="background-color: #dfdfdf; border: 1px solid #cccccc">
						<td colspan="2">
							<input
								type="button"
								name="Gravar"
								value="Gravar"
								onclick="gravarEscolas();"
							/>
							<input
								type="button"
								name="Fechar"
								value="Fechar"
								onclick="fecharJanela();"
							/>
						</td>
					</tr>
					<tr>
						<td>
							<input type="checkbox" onclick="javascript: marcarTodosChecks( 'marcarTodos', 'entid[]' );" name="marcarTodos" id="marcarTodos" />
						</td>
						<td><label style="float: left;" for="marcarTodos">SELECIONAR TODAS</label><span style="font-weight: bold; float: right;"> Total de escolas: <?php echo $qtdEscolas ?> </span></td>
					</tr>
					<?php $cor = ""; ?>
					<?php foreach ( $escolas as $mundescricao => $escolasMunicipio ) : ?>
						<tr bgcolor="#bfbfbf">
							<td colspan="2">
								<?= $mundescricao ?>
							</td>
						</tr>
						<?php foreach ( $escolasMunicipio as $escola ) : ?>
							<?php $cor = $cor == "#f0f0f0" ? "#e0e0e0" : "#f0f0f0"; ?>
							<tr bgcolor="<?= $cor ?>">
								<td width="10">
									<input
										type="checkbox"
										name="entid[]"
										value="<?= $escola['entid'] ?>"
										<?php echo in_array($escola['entid'], $entids)     ? "checked=\"checked\""   : "" ?>
										<?php echo in_array($escola['entid'], $utilizados) ? "checked=\"checked\" onclick=\"this.checked = 'checked'; alert('Escola sendo utilizada em alguma sub-ação');return false;\"" : "" ?>
									/>
								</td>
								<td>
									<?= $escola['entnome'] ?>
								</td>
							</tr>
						<?php endforeach; ?>
					<?php endforeach; ?>					
					<tr style="background-color: #dfdfdf; border: 1px solid #cccccc">
						<td colspan="2">
							<input
								type="button"
								name="Gravar"
								value="Gravar"
								onclick="gravarEscolas();"
							/>
							<input
								type="button"
								name="Fechar"
								value="Fechar"
								onclick="fecharJanela();"
							/>
						</td>
					</tr>
				<?php else: ?>
					<tr bgcolor="#efefef">
						<td style="color:#dd2020;text-align:center;" colspan="2">
							<br/><br/>
							Não há escolas disponíveis
							<br/><br/>
						</td>
					</tr>
				<?php endif; ?>
			</table>
		</form>
	</body>
</html>