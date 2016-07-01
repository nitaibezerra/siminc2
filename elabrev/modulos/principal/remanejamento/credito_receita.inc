<?php

/*
	Sistema Simec
	Setor responsável: SPO-MEC
	Desenvolvedor: Equipe Consultores Simec
	Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
	Programador: Renan de Lima Barbosa (renandelima@gmail.com)
	Módulo: credito_receita.inc
	Finalidade: manutenção de receitas proprias
*/

######################################
#                                    #
#       ABREVIAÇÕES UTILIZADAS       #
#	                                 #
#  tc	tipo de crédito              #
#  dot  dotação inicial              #
#  acr  acrescimo	                 #
#  red  redução                      #
#  out  outros(as)                   #
#                                    #
######################################

require( 'credito_funcoes.inc' );

$edicao_ativa = cre_edicao_ativa();

if ( $_REQUEST['tcrid'] && $_REQUEST['unicod'] )
{
	$tcrid = $_REQUEST['tcrid'];
	$unicod = $_REQUEST['unicod'];
	$_SESSION['creditos_adicionais_tcrid'] = $tcrid;
	$_SESSION['creditos_adicionais_unicod'] = $unicod;
}
else
{
	$tcrid = $_SESSION['creditos_adicionais_tcrid'];
	$unicod = $_SESSION['creditos_adicionais_unicod'];
}

// exibe cabecalho primeiro para mostrar animação de "carregando"
include APPRAIZ . 'includes/cabecalho.inc';

//captura dados do tipo de crédito

require_once 'tipocredito_funcoes.inc';
$tc = tc_pegar( $tcrid );
$tc['tcrnaturezapropria'] = $tc['tcrnaturezapropria'] == 't';

$ano = $_SESSION['exercicio'];

// ---------- PROCESSAMENTO DOS DADOS DO FORMULÁRIO

if ( $edicao_ativa && $tc['tcrnaturezapropria'] && $_REQUEST['submetido'] )
{

	// remove receitas da unidade e do tipo em questão
	$sql = <<<EOT
		delete
		from elabrev.receitaunidade
		where
			rcuano = '$ano' and
			unicod = '$unicod' and
			tcrid = $tcrid and
			mcrid = {$_SESSION['credito_mcrid']}
EOT;
	$db->executar( $sql );
	
	$sql_base = <<<EOT
		insert into elabrev.receitaunidade
		( rcuano, unicod, unitpocod, tcrid, foncod, nrcid, rcuacrescimo, rcureducao, mcrid )
		values
		( '%s', '%s', '%s', %d, '%s', %d, %d, %d, %d )
EOT;
	
	$_REQUEST['receita'] = is_array( $_REQUEST['receita'] ) ? $_REQUEST['receita'] : array();
	foreach ( $_REQUEST['receita'] as $rcuid => $dados_receita )
	{
		$rcuano = $ano;
		$unicod = $unicod;
		$unitpocod = 'U';
		$tcrid = $tcrid;
		$nrcid = $dados_receita['rec'];
		$foncod = $dados_receita['fon'];
		$rcuacrescimo = cre_pegar_numero_cru( $dados_receita['acr'] );
		$rcureducao = cre_pegar_numero_cru( $dados_receita['red'] );
		if ( $rcuacrescimo > 0 || $rcureducao > 0 )
		{
			$sql = sprintf(
				$sql_base,
					$rcuano,
					$unicod,
					$unitpocod,
					$tcrid,
					$foncod,
					$nrcid,
					$rcuacrescimo,
					$rcureducao,
					$_SESSION['credito_mcrid']
			);
			$db->executar( $sql );
		}
	}
	
	$db->commit();
}

// ---------- FIM PROCESSAMENTO DOS DADOS DO FORMULÁRIO

$sql = "select unidsc from public.unidade where unicod = '" . $unicod . "'";
$unidsc = $db->pegaUm( $sql );

// captura as receitas existentes para o tipo de credito e unidade em questão
$receitas = cre_pegar_receitas( $tcrid, $unicod );

/*
$sql = <<<EOT
	select
		nrccod as codigo,
		nrcid || '.' || nrcdsc as dsc
	from
		public.naturezareceita
	where
		nrcstatus = 'A' and
		salcod = '00'
	order by
		nrccod
EOT;
$naturezas = $db->carregar( $sql );
$naturezas = $naturezas ? $naturezas : array();
*/
/*
$sql = <<<EOT
	select
		f.foncod,
		sum( d.dpavalor ) as dot
	from elabrev.despesaacao d
		inner join monitora.acao a on
			a.acaid = d.acaidloa
		inner join elabrev.propostaorcamento p on
			p.ppoid = d.ppoid
		inner join public.fonterecurso f on
			f.foncod = d.foncod
	where
		d.tcrid is null and
		p.tppid = 3 and
		p.ppoano = '$ano' and
		a.prgano = '$ano' and
		a.unicod = '$unicod'
	group by
		f.foncod
	order by
		f.foncod
EOT;
*/
$ppoid = cre_pegar_proposta_ativa();

$sql = <<<EOT
	select
		f.foncod,
		n.nrccod,
		r.rcdvalor as valor
	from elabrev.receitaunidadedotacao r
		inner join public.fonterecurso f on
			f.foncod = r.foncod
		inner join public.naturezareceita n on
			n.nrcid = r.nrcid
	where
		r.rcdano = '$ano' and
		r.unicod = '$unicod'
EOT;
$dados_dot = $db->carregar( $sql );
$dados_dot = $dados_dot ? $dados_dot : array();

$sql = <<<EOT
	select
		r.foncod,
		n.nrccod,
		sum( r.rcuacrescimo ) as valor
	from elabrev.receitaunidade r
		inner join public.naturezareceita n on
			n.nrcid = r.nrcid
	where
		r.rcuano = '$ano' and
		r.tcrid != $tcrid
	group by
		r.foncod,
		n.nrccod
EOT;
$dados_out_acr = $db->carregar( $sql );
$dados_out_acr = $dados_out_acr ? $dados_out_acr : array();

$sql = <<<EOT
	select
		f.foncod,
		n.nrccod,
		sum( r.rcureducao ) as valor
	from elabrev.receitaunidade r
		inner join public.fonterecurso f on
			f.foncod = r.foncod
		inner join public.naturezareceita n on
			n.nrcid = r.nrcid
	where
		r.rcuano = '$ano' and
		r.unicod = '$unicod'
	group by
		f.foncod,
		n.nrccod,
		r.unicod,
		r.tcrid
EOT;
$dados_out_red = $db->carregar( $sql );
$dados_out_red = $dados_out_red ? $dados_out_red : array();

// ----- SQLs para campos de nova receita

// sql para naturezas de receita
//$sql_natureza = <<<EOT
//	select
//		nrccod as codigo,
//		nrcdsc as descricao
//	from
//		public.naturezareceita
//	where
//		nrcstatus = 'A' and
//		salcod = '00'
//	order by
//		nrccod
//EOT;
/* Alterado por Solicitação do NAGIB pois precisa gerar receita com o Elemento de despesa*/
$sql_natureza = <<<EOT
	select
		nrccod as codigo,
		nrcdsc as descricao
	from
		public.naturezareceita
	where
		nrcstatus = 'A' 
	order by
		nrccod
EOT;

// sql para fonte
$sql_fonte = <<<EOT
	select
		foncod as codigo,
		fondsc as descricao
	from public.fonterecurso f
	where
		fonstatus = 'A' and
		( grfid = '2' or grfid = '6' or foncod = '175' or foncod = '150' )
	order by foncod
EOT;

/*$sql_natureza_js = <<<EOT
	select
		nrcid as id,
		nrccod as codigo,
		nrcdsc as descricao
	from
		public.naturezareceita
	where
		nrcstatus = 'A' and
		salcod = '00'
	order by
		nrccod
EOT; */
/* Alterado por Solicitação do NAGIB pois precisa gerar receita com o Elemento de despesa*/
$sql_natureza_js = <<<EOT
	select
		nrcid as id,
		nrccod as codigo,
		nrcdsc as descricao
	from
		public.naturezareceita
	where
		nrcstatus = 'A' 
	order by
		nrccod
EOT;

$naturezas = $db->carregar( $sql_natureza_js );
$naturezas = $naturezas ? $naturezas : array();
$fontes = $db->carregar( $sql_fonte );
$fontes = $fontes ? $fontes : array();

print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, 'Adição de Receita para Créditos Adicionais' );

// captura nome do momento de crédito
$sql = "select mcrdsc from elabrev.momentocredito where mcrid = " . $_SESSION['credito_mcrid'];
$mcrdsc = $db->pegaUm( $sql );

?>
<script type="text/javascript">
	
	function voltarEscolher()
	{
		document.escolher.escolher.value = '1';
		document.escolher.submit();
	}
	
</script>
<form action="?modulo=principal/remanejamento/credito&acao=A" method="post" name="escolher">
	<input type="hidden" name="escolher" value="1"/>
	<table class="tabela" width="90%" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" style="border-bottom: 0px;">
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Momento:
			</td>
			<td width="80%">
				<?= $mcrdsc ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Tipo de Crédito:
			</td>
			<td width="80%">
				<input type="hidden" name="tcrid" value="<?php echo $tc['tcrid']; ?>"/>
				<b><?= $tc['tcrcod'] ?></b> - <?= $tc['tcrdsc'] ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Unidade:
			</td>
			<td width="80%">
				<input type="hidden" name="unicod" value="<?php echo $unicod; ?>"/>
				<b><?= $unicod ?></b> - <?= $unidsc ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				&nbsp;
			</td>
			<td width="80%">
				<input type="button" onclick="voltarEscolher();" name="voltar" value="Voltar para tela inicial de Créditos Adicionais" />
			</td>
		</tr>
		<?php if ( count( $erro ) ) : ?>
			<tr>
				<td colspan="2" align="center" class="SubTituloDireita">
					<b><font color="#ff0000">Dados não gravados</font>! Ocorreu algum erro no sistema.</b>
				</td>
			</tr>
		<?php endif; ?>
	</table>
</form>
<?php if ( !$tc['tcrnaturezapropria'] ) : ?>
	<table class="tabela" width="90%" cellSpacing="1" cellPadding="3" bgcolor="#f5f5f5" align="center" style="border-top: 0px;">
		<tr>
			<td class="SubTituloDireita" style="text-align:center;">
				&nbsp;
				<br/>
				<font color="#ff0000">Este tipo de crédito não aceita receitas próprias.</font>
				<br/>
				&nbsp;
			</td>
		</tr>
	</table>
<?php else : ?>
	<script type="text/javascript">
		
		// ----- VARIÁVEIS -----
		
		// armazena as receitas já cadastradas
		// utiliza somente para verifica se existe a receita quando o usuario tenta adicionar uma nova
		var receita = new Array();
		
		/*
			nrcid.foncod => (
				'dot' => 0
				'out_acr' => 0
				'out_red' => 0
				'acr' => 0
				'red' => 0
			)
		*/
		var valores = new Array();
		
		var naturezas = new Array();
		var fontes = new Array();
		
		<?php foreach ( $naturezas as $item ) : ?>
			naturezas[<?php echo $item['codigo']; ?>] = '<?php echo $item['id'] . '.' . $item['descricao']; ?>';
		<?php endforeach; ?>
		<?php foreach ( $fontes as $item ) : ?>
			fontes[<?php echo $item['codigo']; ?>] = '<?php echo $item['descricao']; ?>';
		<?php endforeach; ?>
		
		var dados_dot = new Array();
		var dados_out_acr = new Array();
		var dados_out_red = new Array();
		
		<?php /*
		<?php reset( $naturezas ); ?>
		<?php reset( $fontes ); ?>
		<?php foreach ( $naturezas as $nat ) : ?>
			<?php foreach ( $fontes as $fon ) : ?>
				<?php $id = $fon['codigo']  . '.' . $nat['codigo']; ?>
				dot_fonte['<?php echo $id; ?>'] = 0;
				dados_out_acr['<?php echo $id; ?>'] = 0;
				dados_out_red['<?php echo $id; ?>'] = 0;
			<?php endforeach; ?>
		<?php endforeach; ?>
		*/ ?>
		
		<?php reset( $dados_dot ); ?>
		<?php foreach ( $dados_dot as $item ) : ?>
			<?php $id = $item['foncod']  . '.' . $item['nrccod']; ?>
			dados_dot['<?php echo $id; ?>'] = <?php echo (integer) $item['valor']; ?>;
		<?php endforeach; ?>
		
		<?php foreach ( $dados_out_acr as $item ) : ?>
			<?php $id = $item['foncod']  . '.' . $item['nrccod']; ?>
			dados_out_acr['<?php echo $id; ?>'] = <?php echo (integer) $item['valor']; ?>;
		<?php endforeach; ?>
		
		<?php foreach ( $dados_out_red as $item ) : ?>
			<?php $id = $item['foncod']  . '.' . $item['nrccod']; ?>
			dados_out_red['<?php echo $id; ?>'] = <?php echo (integer) $item['valor']; ?>;
		<?php endforeach; ?>
		
		// ----- FUNÇÕES -----
		
		function enviarReceitas()
		{
			document.receitas.submit();
		}
		
		function formatarNumero( numero )
		{
			var sinal = numero < 0 ? '-' : '';
			numero += '';
			if ( sinal == '-' )
			{
				numero = numero.substring( 1 );
			}
			if ( numero.length < 4 )
			{
				return sinal + numero;
			}
			var casas = new Array();
			var chave = 0;
			for ( var chave = -1, j = 0, i = numero.length - 1; i >= 0; i--, j++ )
			{
				if ( j % 3 == 0 )
				{
					chave++;
					casas[chave] = '';
				}
				casas[chave] = numero.charAt( i ) + casas[chave];
			}
			var resultado = new Array();
			for ( var i = casas.length - 1; i >= 0; i-- )
			{
				resultado[resultado.length] = casas[i];
			}
			return sinal + resultado.join( '.' );
		}
		
		function pegarInteiro( valor )
		{
			if ( !valor )
			{
				return 0;
			}
			valor += '';
			valor = valor.replace( /\./g, '' );
			while ( valor.charAt( 0 ) == '0' )
			{
				valor = valor.substring( 1 );
			}
			return valor == '' ? 0 : parseInt( valor );
		}
		
		function atualizaValorDinheiro( input )
		{
			input.value = formatarNumero( pegarInteiro( input.value ) );
		}
		
		function atualizaValorDinheiroAcr( input, chave )
		{
			atualizaValorDinheiro( input );
			valores[chave][3] = pegarInteiro( input.value );
			atualizarValores();
		}
		
		function atualizaValorDinheiroRed( input, chave )
		{
			atualizaValorDinheiro( input );
			valores[chave][4] = pegarInteiro( input.value );
			atualizarValores();
		}
		
		function atualizarValores()
		{
			var dot = 0;
			var out_acr = 0;
			var out_red = 0;
			var acr = 0;
			var red = 0;
			var saldo = 0;
			
			var dot_item = 0;
			var out_acr_item = 0;
			var out_red_item = 0;
			var acr_item = 0;
			var red_item = 0;
			var saldo_item = 0;
			
			var span = null;
			var dados = null;
			var chave = '';
			var nrcid = '';
			var foncod = '';
			
			for( chave in valores )
			{
				dados = chave.split( '.' );
				nrcid = dados[0];
				foncod = dados[1];
				if ( !nrcid || !foncod )
				{
					continue;
				}
				
				dot_item = valores[chave][0];
				out_acr_item = valores[chave][1];
				out_red_item = valores[chave][2];
				acr_item = valores[chave][3];
				red_item = valores[chave][4];
				
				saldo_item = dot_item + out_acr_item - out_red_item + acr_item - red_item;
				saldo += saldo_item;
				
				dot += dot_item;
				out_acr += out_acr_item;
				out_red += out_red_item;
				acr += acr_item;
				red += red_item;
				
				span = document.getElementById( chave + '_span_saldo' );
				if ( span )
				{
					span.innerHTML = formatarNumero( pegarInteiro( saldo_item ) );
					span.style.color = saldo_item < 0 ? '#ff0000' : '#0000ff';
				}
				
			}
			
			span = document.getElementById( 'span_dot_geral' );
			span.innerHTML = formatarNumero( pegarInteiro( dot ) );
			
			span = document.getElementById( 'span_out_acr_geral' );
			span.style.color = '#0000ff';
			span.innerHTML = formatarNumero( pegarInteiro( out_acr ) );
			
			span = document.getElementById( 'span_out_red_geral' );
			span.style.color = '#ff0000';
			span.innerHTML = formatarNumero( pegarInteiro( out_red ) );
			
			span = document.getElementById( 'span_acr_geral' );
			span.style.color = '#0000ff';
			span.innerHTML = formatarNumero( pegarInteiro( acr ) );
			
			span = document.getElementById( 'span_red_geral' );
			span.style.color = '#ff0000';
			span.innerHTML = formatarNumero( pegarInteiro( red ) );
			
			span = document.getElementById( 'span_saldo_geral' );
			span.style.color = saldo < 0 ? '#ff0000' : '#0000ff';
			span.innerHTML = formatarNumero( pegarInteiro( saldo ) );
			
		}
		
		// ---------- FUNÇÕES PARA MANIPULAÇÂO DE ITENS NOVOS (despesas que não existiam inicialmente)
		
		
		function novoCredito_inserirDoFormulario()
		{
			var nrccod = document.getElementById( 'nova_natureza' ).value;
			if ( !nrccod )
			{
				alert( 'Preencha a Natureza' );
				var input = document.getElementById( 'nova_natureza' );
				input.focus();
				input.select();
				return;
			}
			if ( !naturezas[nrccod] )
			{
				alert( 'Natureza inválida' );
				var input = document.getElementById( 'nova_natureza' );
				input.focus();
				input.select();
				return;
			}
			var dados = naturezas[nrccod].split( '.' );
			var nrcid = dados[0];
			var nrcdsc = dados[1];
			
			var foncod = document.getElementById( 'nova_fonte' ).value;
			if ( !foncod )
			{
				alert( 'Preencha a Fonte' );
				var input = document.getElementById( 'nova_fonte' );
				input.focus();
				input.select();
				return;
			}
			if ( !fontes[foncod] )
			{
				alert( 'Fonte inválida' );
				var input = document.getElementById( 'nova_fonte' );
				input.focus();
				input.select();
				return;
			}
			
			var chave = nrcid + '.' + foncod;
			if ( receita[chave] )
			{
				alert( 'Natureza de receita já está presente na tabela' );
				return;
			}
			
			var acr = document.getElementById( 'novo_acr' ).value;
			var red = document.getElementById( 'nova_red' ).value;
			
			novoCredito_inserir( nrcid, nrccod, nrcdsc, foncod, acr, red );
			
			document.getElementById( 'nova_natureza' ).value = '';
			document.getElementById( 'nova_fonte' ).value = '';
			document.getElementById( 'novo_acr' ).value = '0';
			document.getElementById( 'nova_red' ).value = '0';
			var input = document.getElementById( 'nova_natureza' );
				input.focus();
				input.select();
			
			atualizarValores();
		}
		
		function novoCredito_criarCelula( tr, nome, valor, align, cor, classe )
		{
			var celula = tr.insertCell( tr.cells.length );
			//celula.style.textAlign = align;
			celula.style.color = cor;
			celula.className = classe;
			celula.appendChild( document.createTextNode( valor ) );
			celula.appendChild( novoItem_CriarHidden( 'extra[' + nome + '][]', valor ) );
		}
		
		var count = 0;
		function novoCredito_inserir( nrcid, nrccod, nrcdsc, foncod, acr, red )
		{
			count++;
			var tabela = document.getElementById( 'tabela_receita' );
			var chave = nrcid + '.' + foncod;
			
			var dot_inicial = 0;
			var outros_acr = 0;
			var outros_red = 0;
			
			if ( dados_dot[foncod] )
			{
				dot_inicial = pegarInteiro( dados_dot[foncod] );
			}
			
			var chave_out = foncod + '.' + nrccod;
			if ( dados_out_acr[chave_out] )
			{
				outros_acr = pegarInteiro( dados_out_acr[chave_out] );
			}
			if ( dados_out_red[chave_out] )
			{
				outros_red = pegarInteiro( dados_out_red[chave_out] );
			}
			
			valores[chave] = new Array(
				dot_inicial,
				outros_acr,
				outros_red,
				pegarInteiro( acr ),
				pegarInteiro( red )
			);
			
			receita[chave] = true;
			
			var id = nrccod + '.' + foncod;
			
			// cria linha
			var tr = tabela.insertRow( tabela.rows.length );
				tr.id = id;
				
			// adiciona celulas de descrição da natureza de receita
			var celulaNat = tr.insertCell( tr.cells.length );
				celulaNat.style.textAlign = 'left';
				celulaNat.appendChild( document.createTextNode( nrccod + ' - ' + nrcdsc ) );
			
			// adiciona celulas de descrição da fonte
			var inputFon = document.createElement( 'input' );
				inputFon.type = 'hidden';
				inputFon.name = 'receita[novo_' + count + '][fon]';
				inputFon.value = foncod;
				
			// adiciona celulas de descrição da receita
			var inputrec = document.createElement( 'input' );
				inputrec.type = 'hidden';
				inputrec.name = 'receita[novo_' + count + '][rec]';
				inputrec.value = nrcid;
				
				celulaNat.appendChild( inputrec );
				
			var celulaFon = tr.insertCell( tr.cells.length );
				celulaFon.style.textAlign = 'center';
				celulaFon.appendChild( document.createTextNode( foncod ) );
				celulaFon.appendChild( inputFon );
			
			// adiciona dotação
			var celulaDot = tr.insertCell( tr.cells.length );
				celulaDot.style.textAlign = 'right';
				celulaDot.appendChild( document.createTextNode( formatarNumero( dot_inicial ) ) );
			
			// adiciona outros acrescimos
			var celulaOutAcr = tr.insertCell( tr.cells.length );
				celulaOutAcr.style.textAlign = 'right';
				celulaOutAcr.style.color = '#000050';
				celulaOutAcr.appendChild( document.createTextNode( formatarNumero( outros_acr ) ) );
			
			// adiciona outras reducoes
			var celulaOutRed = tr.insertCell( tr.cells.length );
				celulaOutRed.style.textAlign = 'right';
				celulaOutRed.style.color = '#500000';
				celulaOutRed.appendChild( document.createTextNode( formatarNumero( outros_red ) ) );
			
			// adiciona celula de acrescimo
			var inputAcr = document.createElement( 'input' );
				inputAcr.id = id + '_acr';
				inputAcr.type = 'text';
				inputAcr.name = 'receita[novo_' + count + '][acr]';
				inputAcr.value = formatarNumero( pegarInteiro( acr ) );
				inputAcr.style.textAlign = 'right';
				inputAcr.size = '12';
				inputAcr.className = 'normal';
				inputAcr.onkeyup = new Function( ' atualizaValorDinheiroAcr( this, \'' + chave + '\' ); ' );
				inputAcr.style.color = '#000050';
			var celulaAcr = tr.insertCell( tr.cells.length );
				celulaAcr.style.textAlign = 'right';
				celulaAcr.appendChild( inputAcr );
			
			// adiciona celula de reducao
			var inputRed = document.createElement( 'input' );
				inputRed.id = id + '_red';
				inputRed.type = 'text';
				inputRed.name = 'receita[novo_' + count + '][red]';
				inputRed.value = formatarNumero( pegarInteiro( red ) );
				inputRed.style.textAlign = 'right';
				inputRed.size = '12';
				inputRed.className = 'normal';
				inputRed.onkeyup = new Function( ' atualizaValorDinheiroRed( this, \'' + chave + '\' ); ' );
				inputRed.style.color = '#500000';
			var celulaRed = tr.insertCell( tr.cells.length );
				celulaRed.style.textAlign = 'right';
				celulaRed.appendChild( inputRed );
			
			var spanSaldo = document.createElement( 'span' );
			    spanSaldo.id = chave + '_span_saldo';
			var celulaSaldo = tr.insertCell( tr.cells.length );
				celulaSaldo.style.textAlign = 'right';
				celulaSaldo.appendChild( spanSaldo );
			
			// editar
			var imgEditar = document.createElement( 'img' );
				imgEditar.src = '/imagens/check_p.gif';
				imgEditar.style.border = 'none';
				imgEditar.style.padding = '0px';
				imgEditar.style.margin = '0px';
				imgEditar.alt = 'Alterar Receita';
				imgEditar.title = 'Alterar Receita';
				imgEditar.onclick = "";
			var linkEditar = document.createElement( 'a' );
			    linkEditar.href = 'javascript:novoCredito_editar( \'' + id + '\' );';
			    linkEditar.title = 'remover';
			    linkEditar.appendChild( imgEditar );
			var celulaEditar = tr.insertCell( tr.cells.length );
				celulaEditar.style.textAlign = 'center';
				celulaEditar.appendChild( linkEditar );
			
			// remover
			var imgRemover = document.createElement( 'img' );
				imgRemover.src = '/imagens/exclui_p.gif';
				imgRemover.style.border = 'none';
				imgRemover.style.padding = '0px';
				imgRemover.style.margin = '0px';
				imgRemover.alt = 'Remover Receita';
				imgRemover.title = 'Remover Receita';
				imgRemover.onclick = "";
			var linkRemover = document.createElement( 'a' );
			    linkRemover.href = 'javascript:novoCredito_remover( \'' + id + '\' );';
			    linkRemover.title = 'remover';
			    linkRemover.appendChild( imgRemover );
			var celulaRemover = tr.insertCell( tr.cells.length );
				celulaRemover.style.textAlign = 'center';
				celulaRemover.appendChild( linkRemover );
			
			novoCredito_AtualizarCor();
		}
		
		function novoCredito_AtualizarCor()
		{
			var tabela = document.getElementById( 'tabela_receita' );
			var quantidade_tr = tabela.rows.length;
			var cor = '';
			for ( var i = 0; i < quantidade_tr; i++ )
			{
				var tr = tabela.rows[i];
				cor = cor == '' ? '#e9e9e9' : '' ;
				tr.style.backgroundColor = cor;
				tr.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
				tr.onmouseout = new Function( ' this.style.backgroundColor = "' + cor + '"; ' );
			}
		}
		
		function novoCredito_remover( id )
		{
			var dados = id.split( '.' );
			var nrccod = dados[0];
			var foncod = dados[1];
			if ( !confirm( 'Deseja realmente excluir receita de Natureza ' + nrccod + ' e Fonte ' + foncod  + '?' ) )
			{
				return;
			}
			var tr = document.getElementById( id );
			if ( tr )
			{
				tr.parentNode.removeChild( tr );
			}
			
			var dados = naturezas[nrccod].split( '.' );
			var nrcid = dados[0];
			var nrcdsc = dados[1]; 
			var chave = nrcid + '.' + foncod;
			receita[chave] = false;
			valores[chave][0] = 0;
			valores[chave][1] = 0;
			valores[chave][2] = 0;
			valores[chave][3] = 0;
			valores[chave][4] = 0;
			
			atualizarValores();
		}
		
		
		function novoCredito_editar( id )
		{
			var dados = id.split( '.' );
			var nrccod = dados[0];
			var foncod = dados[1];
			var acr = document.getElementById( id + '_acr' ).value;
			var red = document.getElementById( id + '_red' ).value;
			
			var tr = document.getElementById( id );
			if ( tr )
			{
				tr.parentNode.removeChild( tr );
			}
			
			document.getElementById( 'nova_natureza' ).value = nrccod;
			document.getElementById( 'nova_fonte' ).value = foncod;
			document.getElementById( 'novo_acr' ).value = acr;
			document.getElementById( 'nova_red' ).value = red;
			var input = document.getElementById( 'nova_natureza' );
				input.focus();
				input.select();
			
			var dados = naturezas[nrccod].split( '.' );
			var nrcid = dados[0];
			var nrcdsc = dados[1]; 
			var chave = nrcid + '.' + foncod;
			receita[chave] = false;
			valores[chave][0] = 0;
			valores[chave][1] = 0;
			valores[chave][2] = 0;
			valores[chave][3] = 0;
			valores[chave][4] = 0;
			
			atualizarValores();
		}
		
		function novaReceita_moveProximoCampo( event, campo )
		{
			if ( event.keyCode != 13 )
			{
				return;
			}
			switch ( campo )
			{
				case 'natureza':
					var input = document.getElementById( 'nova_fonte' );
					break;
				case 'fonte':
					var input = document.getElementById( 'novo_acr' );
					break;
				case 'acr':
					var input = document.getElementById( 'nova_red' );
					break;
				case 'red':
					novoCredito_inserirDoFormulario();
				default: 
					return;
			}
			input.focus();
			input.select();
		}
		
		
	</script>
	
	<form action="" method="post" name="receitas">
		<input type="hidden" name="submetido" value="1"/>
		<table class="tabela" width="90%" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" style="">
			<thead>
				<tr bgcolor="#dfdfdf">
					<td align="left" title="Natureza de Receita"><b>Natureza de Receita</b></td>
					<td align="center" title="Fonte de Recurso" width="90"><b>Fonte</b></td>
					<td align="right" title="Dotação Inicial" width="100"><b>Dotação Inicial</b></td>
					<td align="right" title="Acréscimo" width="130"><b>Outros Acréscimos</b></td>
					<td align="right" title="Redução" width="130"><b>Outras Reduções</b></td>
					<td align="right" title="Acréscimo" width="100"><b>Acréscimo</b></td>
					<td align="right" title="Redução" width="100"><b>Redução</b></td>
					<td align="right" title="Saldo" width="100"><b>Saldo</b></td>
					<td width="35">&nbsp;</td>
					<td width="35">&nbsp;</td>
				</tr>
			</thead>
			<tbody id="tabela_receita">
				<?php foreach ( $receitas as $receita ) : ?>
					<?php $id = $receita['nrccod'] . '.' . $receita['foncod']; ?>
					<tr id="<?php echo $id; ?>">
						<td align="left" title="Natureza de Receita">
							<script type="text/javascript">
								
								<?php $chave = $receita['nrcid'] . '.' . $receita['foncod']; ?>
								
								receita['<?php echo $chave; ?>'] = true;
								
								valores['<?php echo $chave; ?>'] = new Array(
									<?php echo (integer) $receita['dot']; ?>,
									<?php echo (integer) $receita['outros_acr']; ?>,
									<?php echo (integer) $receita['outros_red']; ?>,
									<?php echo (integer) $receita['rcuacrescimo']; ?>,
									<?php echo (integer) $receita['rcureducao']; ?>
								);
								
							</script>
							<input type="hidden" name="receita[<?php echo $receita['rcuid']; ?>][rec]" value="<?php echo $receita['nrcid']; ?>"/>
							<?php echo $receita['nrccod']; ?>
							-
							<?php echo $receita['nrcdsc']; ?>
						</td>
						<td align="center" title="Fonte de Recurso">
							<input type="hidden" name="receita[<?php echo $receita['rcuid']; ?>][fon]" value="<?php echo $receita['foncod']; ?>"/>
							<?php echo $receita['foncod']; ?>
						</td>
						<td align="right" title="Dotação Inicial">
							<input type="hidden" name="receita[<?php echo $receita['rcuid']; ?>][fon]" value="<?php echo $receita['foncod']; ?>"/>
							<?php echo number_format( $receita['dot'], 0, ',', '.' ); ?>
						</td>
						<td align="right" title="Outros Acréscimos" style="color:#000050;">
							<?php echo number_format( $receita['outros_acr'], 0, ',', '.' ); ?>
						</td>
						<td align="right" title="Outras Reduções" style="color:#500000;">
							<?php echo number_format( $receita['outros_red'], 0, ',', '.' ); ?>
						</td>
						<td align="right" title="Acréscimo">
							<input
								type="text"
								id="<?php echo $id; ?>_acr"
								name="receita[<?php echo $receita['rcuid']; ?>][acr]"
								value="<?php echo number_format( $receita['rcuacrescimo'], 0, ',', '.' ); ?>"
								onkeyup="atualizaValorDinheiroAcr( this, '<?php echo $chave; ?>' );"
								style="text-align:right;color:#000050;"
								size="12"
								class="normal"
								<?php echo $edicao_ativa ? '' : 'disabled="disabled"'; ?>
							/>
						</td>
						<td align="right" title="Redução">
							<input
								type="text"
								id="<?php echo $id; ?>_red"
								name="receita[<?php echo $receita['rcuid']; ?>][red]"
								value="<?php echo number_format( $receita['rcureducao'], 0, ',', '.' ); ?>"
								onkeyup="atualizaValorDinheiroRed( this, '<?php echo $chave; ?>' );"
								style="text-align:right;color:#500000;"
								size="12"
								class="normal"
								<?php echo $edicao_ativa ? '' : 'disabled="disabled"'; ?>
							/>
						</td>
						<td align="right" title="Saldo">
							<span id="<?php echo $chave; ?>_span_saldo"></span>
						</td>
						<td align="center">
							<?php if ( $edicao_ativa ) : ?>
								<a href="javascript:novoCredito_editar( '<?php echo $id; ?>' );"><img border="0" src="/imagens/check_p.gif" title="Alterar Receita"/></a>
							<?php else : ?>
								-
							<?php endif; ?>
						</td>
						<td align="center">
							<?php if ( $edicao_ativa ) : ?>
								<a href="javascript:novoCredito_remover( '<?php echo $id; ?>' );"><img border="0" src="/imagens/exclui_p.gif" title="Remover Receita" onlick="novoCredito_remover( '<?php echo $id; ?>' );"/></a>
							<?php else : ?>
								-
							<?php endif; ?>
						</td>
					</tr>
				<?php endforeach; ?>
			</tbody>
			<tfoot>
				<tr bgcolor="#d0d0d0">
					<td colspan="2">&nbsp;</td>
					<td align="right" style="font-weight:bold;" title="Dotação Inicial Total">
						<span id="span_dot_geral"></span>
					</td>
					<td align="right" style="font-weight:bold;" title="Outros Acréscimos Total">
						<span id="span_out_acr_geral"></span>
					</td>
					<td align="right" style="font-weight:bold;" title="Outras Reduções Total">
						<span id="span_out_red_geral"></span>
					</td>
					<td align="right" style="font-weight:bold;" title="Acréscimo Total">
						<span id="span_acr_geral"></span>
					</td>
					<td align="right" style="font-weight:bold;" title="Redução Total">
						<span id="span_red_geral"></span>
					</td>
					<td align="right" style="font-weight:bold;" title="Saldo Total">
						<span id="span_saldo_geral"></span>
					</td>
					<td colspan="2">&nbsp;</td>
				</tr>
				<?php if ( $edicao_ativa ) : ?>
					<tr>
						<td align="right" title="Natureza para nova receita">
							<? $complemento = ' onkeypress="novaReceita_moveProximoCampo( event, \'natureza\' );" '; ?>
							<?php texto_popup( 'nova_natureza', $sql_natureza, 'Natureza', 8, 8, '########', '', $complemento ); ?>
						</td>
						<td align="center" title="Fonte de Recurso para nova receita">
							<? $complemento = ' onkeypress="novaReceita_moveProximoCampo( event, \'fonte\' );" '; ?>
							<?php texto_popup( 'nova_fonte', $sql_fonte, 'Fonte', 3, 4, '###', '', $complemento ); ?>
						</td>
						<td colspan="3">&nbsp;</td>
						<td align="right" title="Acréscimo para nova receita">
							<input
								type="text"
								name="novo_acr"
								id="novo_acr"
								value="0"
								onkeyup="atualizaValorDinheiro( this );"
								onkeypress="novaReceita_moveProximoCampo( event, 'acr' );"
								style="text-align:right;color:#000050;"
								size="12"
								class="normal"
							/>
						</td>
						<td align="right" title="Redução para nova receita">
							<input
								type="text"
								name="novo_red"
								id="nova_red"
								value="0"
								onkeyup="atualizaValorDinheiro( this );"
								onkeypress="novaReceita_moveProximoCampo( event, 'red' );"
								style="text-align:right;color:#500000;"
								size="12"
								class="normal"
							/>
						</td>
						<td align="right">&nbsp;</td>
						<td align="center" colspan="2">
							<input type="button" name="botaoAdicionarNovaReceita" value="Adicionar" onclick="novoCredito_inserirDoFormulario();"/>
						</td>
					</tr>
				<?php endif; ?>
			</tfoot>
		</table>
		<?php if ( $edicao_ativa ) : ?>
			<table class="tabela" width="90%" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" style="border-top:0;">
				<tr>
					<td width="20%" align="right" class="SubTituloDireita">
						&nbsp;
					</td>
					<td width="80%">
						<input type="button" name="botaoGravarReceita" value="Gravar" onclick="enviarReceitas();"/>
					</td>
				</tr>
			</table>
		<?php endif; ?>
	</form>
	<script type="text/javascript">
		novoCredito_AtualizarCor();
		atualizarValores();
	</script>
<?php endif; ?>



