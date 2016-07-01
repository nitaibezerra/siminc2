<?php

	// FUNÇÕES

	define( 'TIPO_PROPOSTA', 1 );
	define( 'TIPO_PROJETO_LEI', 2 );
	define( 'TIPO_LEI', 3 );

	/**
	 * Captura o identificador da ação que contenha os dados indicados pelo parâmetros.
	 *
	 * @param integer $tppid
	 * @param string $unicod
	 * @param string $prgcod
	 * @param string $acacod
	 * @param string $loccod
	 * @return integer
	 */
	function pegar_acaid( $tppid, $unicod, $prgcod, $acacod, $loccod )
	{
		global $db;
		$tabela = $tppid == TIPO_LEI ? 'monitora.acao' : 'elabrev.ppaacao_orcamento';
		$sql =
			" select acaid " .
			" from " . $tabela .
			" where " .
				" unicod = '" . $unicod . "' and " . 
				" prgcod = '" . $prgcod . "' and " . 
				" acacod = '" . $acacod . "' and " . 
				" loccod = '" . $loccod . "' and " . 
				" prgano = '" . $_SESSION['exercicio'] . "' and " .
				" acasnrap = 'f' ";

		return (integer) $db->pegaUm( $sql );
	}
	
	/**
	 * Captura o saldo atual de uma unidade para uma determinada fonte.
	 *
	 * @param string $foncod
	 * @param string $unicod
	 * @return integer
	 */
	function pegar_saldo_unidade_fonte( $foncod, $unicod )
	{
		global $db;
		$sql =
			" select saldo " .
			" from elabrev.v_saldounidadefonte " .
			" where " .
				" unicod = '" . $unicod . "' and " .
				" foncod = '" . $foncod . "' ";
		$saldo = $db->pegaUm( $sql );
		if ( !$saldo && $saldo !== '0' )
		{
			return null;
		}
		return $saldo;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param integer $acaid
	 * @param string $nat
	 * @param string $uso
	 * @param string $fon
	 * @param string $doc
	 * @return integer
	 */
	function pegar_despesa( $tppid, $acaid, $nat, $uso, $fon, $doc )
	{
		global $db;
		$campo_acaid = $tppid == TIPO_LEI ? 'acaidloa' : 'acaid';
		$sql =
			" select coalesce( sum( dpavalor ), 0 ) as valor " .
			" from elabrev.despesaacao " .
				" inner join naturezadespesa using ( ndpid ) " .
				" inner join idoc using ( idoid ) " .
			" where " .
				" foncod = '" . $fon . "' and " .
				$campo_acaid . " = " . $acaid . " and " .
				" ndpcod = '" . $nat . "' and " .
				" iducod = '" . $uso . "' and " .
				" idocod = '" . $doc . "'";
		return (integer) $db->pegaUm( $sql );
	}
	
	/**
	 * Remove todos os itens de despesa de uma ação.
	 *
	 * @param integer $acaid
	 * @param string $nat
	 * @param string $uso
	 * @param string $fon
	 * @param string $doc
	 * @return integer
	 */
	function remover_despesa( $tppid, $ppoid, $acaid, $ung, $sub, $nat, $uso, $fon, $doc )
	{
		global $db;
		static $idoids = array();
		static $npdids = array();
		static $sbaids = array();
		$acaid = (integer) $acaid;
		if ( !isset( $idoids[$doc] ) )
		{
			$idoids[$doc] = pegar_idoid( $doc );
		}
		if ( !isset( $npdids[$nat] ) )
		{
			$npdids[$nat] = pegar_npdid( $nat );
		}

		if ( $ung ) {
			$sql = "select unicod from unidadegestora where ungcod = '" . $ung . "'";
			$unicod = $db->pegaUm( $sql );
		}

		if ( !isset( $sbaids[$sub] ) )
		{
			$sbaids[$sub] = pegar_sbaid( $sub, $unicod );
		}
		
		$campo_acaid = $tppid == TIPO_LEI ? 'acaidloa' : 'acaid';
		$sql =
			" delete from elabrev.despesaacao where " .
				$campo_acaid . " = " . $acaid . " and " .
				" ndpid = " . $npdids[$nat] . " and " .
				" ungcod = '" . $ung . "' and " .
				" sbaid = '" . $sbaids[$sub] . "' and " .
				" iducod = '" . $uso . "' and " .
				" foncod = '" . $fon . "' and " .
				" idoid = " . $idoids[$doc];
		return (boolean) $db->executar( $sql );
	}
	
	/**
	 * Insere um item na tabela elabrev.despesaacao.
	 *
	 * @param integer $acaid
	 * @param string $ndpcod
	 * @param string $iducod
	 * @param string $foncod
	 * @param integer $idocod
	 * @param float $valor
	 * @return boolean
	 */
	function inserir_despesa( $tppid, $ppoid, $acaid, $ung, $sub, $ploid, $ndpcod, $iducod, $foncod, $idocod, $valor )
	{
		global $db;
		static $idoids = array();
		static $npdids = array();
		static $sbaids = array();
		$acaid = (integer) $acaid;
		$idoid = (integer) $idoid;
		if( $ploid == '-' ){
			$ploid = 'null';
		} else {
			$ploid = ($ploid ? $db->pegaUm("select ploid from elabrev.planoorcamentario where plocodigo = '$ploid' and acaid = $acaid") : 'null');
		}
		$valor = (float) $valor;
		if ( !isset( $idoids[$idocod] ) )
		{
			$idoids[$idocod] = pegar_idoid( $idocod );
		}
		if ( !isset( $npdids[$ndpcod] ) )
		{
			$npdids[$ndpcod] = pegar_npdid( $ndpcod );
		}
		
		if ( $ung ) {
			$sql = "select unicod from unidadegestora where ungcod = '" . $ung . "'";
			$unicod = $db->pegaUm( $sql );
		}

		if ( !isset( $sbaids[$sub] ) )
		{
			$sbaids[$sub] = pegar_sbaid( $sub, $unicod );
		}
		
		$campo_acaid = $tppid == TIPO_LEI ? 'acaidloa' : 'acaid';
		$sql = 
			" insert into elabrev.despesaacao " .
			" ( " .
				$campo_acaid . ", ungcod, sbaid, ndpid, iducod, foncod, idoid, dpavalor, ppoid, ploid " .
			" ) values ( " .
				$acaid . ", '" . $ung . "', " . $sbaids[$sub] . "," . $npdids[$ndpcod] . ", '" . $iducod . "', '" . $foncod . "', " . $idoids[$idocod] . ", " . $valor . ", " . $ppoid . ", " . $ploid . " ) ";
		//dbg( $sql );
		if ( !$db->executar( $sql ) )
		{
			dbg( $idocod );
			dbg( $sql, 1 );
		}
		return true;
		return (boolean) $db->executar( $sql );
	}
	
	/**
	 * Captura o idoid a partir de um idococ da tabela idoc.
	 *
	 * @param string $idocod
	 * @return integer
	 */
	function pegar_idoid( $idocod )
	{
		global $db;
		$sql = "select idoid from idoc where idocod = '" . $idocod . "'";

		return (integer) $db->pegaUm( $sql );
	}
	
	/**
	 * Captura o npdid a partir de um npdcod da tabela naturezadespesa.
	 *
	 * @param string $ndpcod
	 * @return integer
	 */
	function pegar_npdid( $ndpcod )
	{
		global $db;
		$sql = "select ndpid from naturezadespesa where ndpcod = '" . $ndpcod . "'";
		return (integer) $db->pegaUm( $sql );
	}
	/**
	 * Captura o sbaid a partir de um sbacod da tabela monitora.pi_subacao.
	 *
	 * @param string $sbacod
	 * @return integer
	 */
	function pegar_sbaid( $sbacod, $unicod ) 
	{
			global $db;
			$sql = "select sbaid from elabrev.subacao where sbacod = '" . $sbacod . "' and unicod = '".$unicod."' and sbastatus = 'A'";
			return (integer) $db->pegaUm( $sql );
	}
	
		function pegar_ploid( $plocodigo )
	{
		global $db;
		$sql = "select de.ploid from elabrev.despesaacao de inner join elabrev.planoorcamentario pl on pl.ploid = de.ploid where pl.plocodigo = '" . $plocodigo . "'";
		return (integer) $db->pegaUm( $sql );
	}
	
	function existePlano( $acaid )
	{
		global $db;
		$total = $db->pegaUm( "select count(plocodigo) from elabrev.planoorcamentario where acaid = $acaid" );
		$boPlocodigo = ($total > 0 ? true : false);
		
		return (boolean) $boPlocodigo;
	}
	
	/**
	 * Captura o tipo da proposta.
	 *
	 * @param integer $$ppoid
	 * @return integer
	 */
	function pegar_tipo_proposta( $ppoid )
	{
		global $db;
		$ppoid = (integer) $ppoid;
		$sql = "select p.tppid from elabrev.propostaorcamento p inner join elabrev.tipoproposta tp on tp.tppid = p.tppid where p.ppoid = '" . $ppoid . "'";
		return (integer) $db->pegaUm( $sql );
	}
	
	/**
	 * Estrutura os dados de entrada em um array organizado. Abaixo a estrutura:
		
		$dados = array(
			'*uni_*pro_*aca_*loc' => array(
				'uni' => '...',
				'pro' => '...',
				'aca' => '...',
				'loc' => '...',
				'valores' => array(
					'nat' => '...',
					'uso' => '...',
					'fon' => '...',
					'doc' => '...',
					'val' => '...',
					'lin' => '...'
				)
			)
		);
		
		Erros retornados:
		
		1 -> má formação em alguma linha, ela não possui 9 itens
		1 -> valores repetidos
		
	 *
	 * @param string $dados_brutos
	 * @return array
	 */
	function processar_entrada( $dados_brutos, $tppid )
	{
		global $ultimo_erro;
		static $ids_universais = array();
		
		$dados = array();
		$dados_linhas = explode( "\n", $dados_brutos );
		foreach ( $dados_linhas as $linha => $dados_linha )
		{
			if ( !$dados_linha )
			{
				continue;
			}
			$dados_linha = explode( "\t", $dados_linha );
			
			$uni = $dados_linha[0];
			$ung = $dados_linha[1];
			$sub = $dados_linha[2];
			$pro = $dados_linha[3];
			$aca = $dados_linha[4];
			$loc = $dados_linha[5];
			$pla = $dados_linha[6];
			$nat = $dados_linha[7];
			$uso = $dados_linha[8];
			$fon = $dados_linha[9];
			$doc = $dados_linha[10];
			$val = (integer) rtrim( str_replace( '.', '', $dados_linha[11] ) );
			
			$acaid = pegar_acaid( $tppid, $uni, $pro, $aca, $loc );
			$boPlano = existePlano( $acaid );
			
			if( $boPlano && empty($pla) ){
				return 'Linha '.( $linha + 1 ).' está faltando o número do plano orçamentário.';
			} 
			
			if ( count( $dados_linha ) != 12 )
			{
				return 'Linha ' . ( $linha + 1 ) . ' está mal formada.';
			}
			
			if( $pla != '-' && strlen($pla) != 4 ){
				return 'Linha ' . ( $linha + 1 ) . ' está com o numero de plano orçamentário incorreto.';
			}
			/* Desconsidera valores iguais ou menores do que zero
			if ( $val <= 0 ) // ignora valores menores que 0
			{
				continue;
			}*/
			
			$id_universal = $uni . ' - ' .$ung . ' - ' . $sub . ' - ' . $pro . ' - ' . $aca . ' - ' . $loc . ' - ' . $pla .  ' - ' . $nat . ' - ' . $uso . ' - ' . $fon . ' - ' . $doc;
			if ( array_key_exists( $id_universal, $ids_universais ) )
			{
				continue;
				/*
				$linha_1 = $ids_universais[$id_universal] + 1;
				$linha_2 = $linha + 1;
				return 'Item ' . $id_universal . ' está repetido. Registros repetidos detectados nas linhas ' . $linha_1 . ' e ' . $linha_2;
				*/
			}
			$ids_universais[$id_universal] = $linha;
						

			$chave_raiz = $uni . '_' . $pro . '_' . $aca . '_' . $loc;
			if ( !array_key_exists( $chave_raiz, $dados ) )
			{
				$dados[$chave_raiz] = array(
					'uni' => $uni,
					'pro' => $pro,
					'aca' => $aca,
					'loc' => $loc,
					'valores' => array()
				);
			}
			$novo_item = array(
				'ung' => $ung,
				'sub' => $sub,
				'pla' => $pla,
				'nat' => $nat,
				'uso' => $uso,
				'fon' => $fon,
				'doc' => $doc,
				'val' => $val
			);
			array_push( $dados[$chave_raiz]['valores'], $novo_item );
		}
		return $dados;
	}

	
	// FIM FUNÇÕES
	
	if ( isset( $_REQUEST['controle'] ) )
	{
		
		$erro = false;
		$erro_msg = '';
		$ppoid = (integer) $_REQUEST['ppoid'];
		$tppid = pegar_tipo_proposta( $ppoid );
		$dados = processar_entrada( $_REQUEST['dados'], $tppid );
		
		if ( is_string( $dados ) ) // verifica se ocorreu erro ao carregar os dados
		{
			$erro = true;
			$erro_msg = str_replace( "'", "\\'", $dados );
		}
		else if ( !$ppoid ) // verifica se a proposta foi escolhida
		{
			$erro = true;
			$erro_msg = 'Erro! Proposta inválida.';
		}
		else foreach ( $dados as $dados_por_acao )
		{
			$uni = $dados_por_acao['uni'];
			$pro = $dados_por_acao['pro'];
			$aca = $dados_por_acao['aca'];
			$loc = $dados_por_acao['loc'];
			$acaid = pegar_acaid( $tppid, $uni, $pro, $aca, $loc );
			if ( !$acaid )
			{
				$erro = true;
				$erro_msg = 'Atenção! Ação da Unidade ' . $uni . ', Programa ' . $pro . ', Ação ' . $aca . ' e Localizador ' . $loc . ' não existe.';
				break;
			}
			foreach ( $dados_por_acao['valores'] as $dados_despesa )
			{
				// captura dados da despesa
				$ung = $dados_despesa['ung'];
				$sub = $dados_despesa['sub'];
				$pla = $dados_despesa['pla'];
				$nat = $dados_despesa['nat'];
				$uso = $dados_despesa['uso'];
				$fon = $dados_despesa['fon'];
				$doc = $dados_despesa['doc'];
				$despesa_atual = $dados_despesa['val'];
				// FIM captura dados da despesa
				
				// realiza operações (verificações) específicas para cada tipo de proposta
				switch ( $tppid )
				{
					case TIPO_PROPOSTA: // verifica se a fonte possui algum limite
						// caso a despesa esteja sendo zerada não é necessário verificar limite
						if ( $despesa_atual > 0 )
						{
							/*$saldo_antigo = pegar_saldo_unidade_fonte( $fon, $uni );
							if ( $saldo_antigo === null )
							{
								$erro = true;
								$erro_msg = 'Atenção! A fonte ' . $fon . ' não possui limite para a Unidade ' . $uni . '.';
								break 3;
							}*/
						}
						// verifica novo saldo
						// despesa antiga é creditado no saldo
						// despesa atual é debitada no saldo
						/*
						$despesa_antiga = pegar_despesa( $acaid, $nat, $uso, $fon, $doc );
						$saldo_novo = $saldo_antigo + $despesa_antiga - $despesa_atual;
						*/
						break;
					case TIPO_PROJETO_LEI:
						break;
					case TIPO_LEI:
						break;
				}
				// FIM realiza operações (verificações) específicas para cada tipo de proposta
				// atualiza despesa
				if ( !remover_despesa( $tppid, $ppoid, $acaid, $ung, $sub, $nat, $uso, $fon, $doc ) )
				{
					$msg_erro = 'Atenção! Ocorreu alguma falha no banco de dados ao tentar remover despesa da Ação da Unidade ' . $uni . ', Programa ' . $pro . ', Ação ' . $aca . ' e Localizador ' . $loc . '.';
					$erro = true;
					break 2;
				}
				if ( $despesa_atual > 0 )
				{
					if ( !inserir_despesa( $tppid, $ppoid, $acaid, $ung, $sub, $pla, $nat, $uso, $fon, $doc, $despesa_atual ) )
					{
						$msg_erro = 'Atenção! Ocorreu alguma falha no banco de dados ao tentar inserir despesa da Ação da Unidade ' . $uni . ', Programa ' . $pro . ', Ação ' . $aca . ' e Localizador ' . $loc . '.';
						$erro = true;
						break 2;
					}
				}
				// FIM atualiza despesa
				
			}
		}
		if ( $erro )
		{
			$db->rollback();
			?>
				<script type="text/javascript" language="javascript">
					alert( '<?php echo $erro_msg; ?>' );
				</script>
			<?
		}
		else
		{
			$db->commit();
			$db->sucesso( 'principal/despesasgerais_sub' );
		}
	}
	
	include APPRAIZ."includes/cabecalho.inc"; 
	
	$sql_proposta = "select ppoid as codigo, ppodsc as descricao from elabrev.propostaorcamento where ppostatus = 'A' and tppid=1 and ppoanoexercicio = '" . ($_SESSION['exercicio']) . "'";
	/* Comentar a linha de cima no momento de proposta
	$sql_proposta = "select ppoid as codigo, ppodsc as descricao from elabrev.propostaorcamento where ppostatus = 'A' and tppid=1 and ppoano = '" . ($_SESSION['exercicio']+1) . "'";
	*/
?>
<br/>
<? 
$db->cria_aba( $abacod_tela, $url, '' );
$menu = array(0 => array("id" => 1, "descricao" => "Demais unidades orçamentárias",   "link" => "/elabrev/elabrev.php?modulo=principal/despesasgerais&acao=A"),
			  1 => array("id" => 2, "descricao" => "Administração direta",			  "link" => "/elabrev/elabrev.php?modulo=principal/despesasgerais_sub&acao=A"),
			  2 => array("id" => 3, "descricao" => "Importar Limites",			  	  "link" => "/elabrev/elabrev.php?modulo=principal/importarLimites&acao=A")
		  	  );
echo montarAbasArray($menu, "/elabrev/elabrev.php?modulo=principal/despesasgerais_sub&acao=A");
monta_titulo( $titulo_modulo, '' );

?>
<form method="post" name="formulario">
	<input type="hidden" name="controle" value="1">
	<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td></td>
			<td>
				layout:&nbsp;&nbsp;
				cod_uo&nbsp;&nbsp;|&nbsp;&nbsp;
				cod_ug&nbsp;&nbsp;|&nbsp;&nbsp;
				cod_subacao&nbsp;&nbsp;|&nbsp;&nbsp;
				cod_programa&nbsp;&nbsp;|&nbsp;&nbsp;
				cod_acao&nbsp;&nbsp;|&nbsp;&nbsp;
				cod_localizador&nbsp;&nbsp;|&nbsp;&nbsp;
				cod_planoorcamento&nbsp;&nbsp;|&nbsp;&nbsp;
				cod_natureza&nbsp;&nbsp;|&nbsp;&nbsp;
				iduso&nbsp;&nbsp;|&nbsp;&nbsp;
				cod_fonte&nbsp;&nbsp;|&nbsp;&nbsp;
				idoc&nbsp;&nbsp;|&nbsp;&nbsp;
				valor
				<br/>
				Um registro por linha separados por tabulação
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Proposta:</td>
			<td align='left' width="90%" class="SubTituloDireita">
				<p style="text-align: left;">
					<?= $db->monta_combo( 'ppoid', $sql_proposta, 'S', 'Escolha a Proposta', '', '', '', '', 'S', '' ); ?>
				</p>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Dados:</td>
			<td align='left' width="90%" class="SubTituloDireita">
				<p style="text-align: left;">
					<? $dados = $_REQUEST['dados']; ?>
					<?= campo_textarea( 'dados', 'S', 'S', 'Dados', 125, 20, null ) ?>
				</p>
			</td>
		</tr>
		<tr bgcolor="#cccccc">
			<td>&nbsp;</td>
			<td>
				<input type="button" name="bsubmeter" value="Salvar" class="botao" onclick="submeter();"/>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">

	/**
	 * Verifica se formulário este correto e o submete.
	 * 
	 * @return void
	 */
	function submeter()
	{
		var formulario = document.formulario;
		formulario.submit();
	}

</script>