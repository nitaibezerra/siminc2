<?php

cte_verificaSessao();



include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
cte_montaTitulo( $titulo_modulo, '' );

$inuid = (integer) $_SESSION["inuid"];
$itrid = cte_pegarItrid( $inuid );

$sql = "
	select iue.entid, upper(e.entnome) as entnome, mu.muncod, mu.mundescricao
	from cte.instrumentounidadeescola iue
		inner join entidade.entidade e on e.entid = iue.entid and (e.entescolanova = false OR e.entescolanova  IS NULL)
       	inner join entidade.endereco ed on ed.entid = e.entid 
       	inner join territorios.municipio mu on mu.muncod = ed.muncod
	where iue.inuid = " . $inuid . "
	group by iue.entid, e.entnome, mu.muncod, mu.mundescricao
	order by mu.mundescricao, e.entnome";

$escolas = $db->carregar( $sql );
$escolas = $escolas ? $escolas : array();
?>
<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<colgroup>
		<col/>
	</colgroup>
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
				<div id="bloco" style="overflow: hidden;">
					<p>
						<a href="javascript: arvore.openAll();">Abrir Todos</a>
						&nbsp;|&nbsp;
						<a href="javascript: arvore.closeAll();">Fechar Todos</a>
					</p>
					<div id="_arvore"></div>
				</div>
			</td>
		</tr>
	</tbody>
</table>
<script type="text/javascript"><!--
	function alterarSubacao( sbaid )
	{
		window.open( "?modulo=principal/par_subacao&acao=A&sbaid=" + sbaid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
	}
	
    function emitirRelatorioPPP(entid)
    {
        return windowOpen( '?modulo=principal/emissaorelatorioppp&acao=A&entid=' + entid, 'emissaorelatorioppp','height=550,width=550,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no' );
    }

	function escolherEscolas()
	{
		return windowOpen( '?modulo=principal/escolasescolha&acao=A','blank','height=400,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}

	function voltar()
	{
		window.location.href = '?modulo=principal/estrutura_avaliacao&acao=A';
	}

	arvore = new dTree( 'arvore' );
	arvore.config.folderLinks = true;
	arvore.config.useIcons = true;
	arvore.config.useCookies = true; 
	arvore.add( 1, -1, "Escolas Atendidas", 'javascript:void(0);' );
	<?php
        $municipios_adicionados = array();
        foreach ( $escolas as $escola ) {

		$entid        = (integer) $escola["entid"];
		$entnome      = $escola["entnome"];
		$muncod       = $escola["muncod"];
		$mundescricao = $escola["mundescricao"];
		$codigo_pai   = 1;
		?>
		<? if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) : ?>
			<? $codigo_pai = "mu_" . $muncod; ?>
			<?php if ( !in_array( $muncod, $municipios_adicionados ) ): ?>
				arvore.add( '<?= $codigo_pai ?>', 1, "<?= $mundescricao ?>", 'javascript:void(0);' );
			<?php endif; ?>
		<?php endif; ?>
		<?php
			array_push( $municipios_adicionados, $muncod );
			$nome = '<a href=\\"?modulo=principal/escola&acao=A&entid=' . $entid . '\\">' . $entnome . '</a>';
			$codigo = 'es_' . $entid;
		?>
			arvore.add( '<?= $codigo ?>', '<?= $codigo_pai ?>', "<?= $nome ?>", 'javascript:void(0);' );
        <?php
		$sql = "
			select
				sa.sbaid,
				sa.sbadsc
			from cte.qtdfisicoano qfa
				inner join cte.subacaoindicador sa on
					sa.sbaid = qfa.sbaid
				inner join cte.acaoindicador ac on
					ac.aciid = sa.aciid
				inner join cte.pontuacao po on
					po.ptoid = ac.ptoid
			where
				qfa.entid = " . $entid . " and
				po.ptostatus = 'A' and
				po.inuid = " . $inuid . "
			group by
				sa.sbaid,
				sa.sbadsc";

		$subacaoes = $db->carregar( $sql );
		$subacaoes = $subacaoes ? $subacaoes : array();
		$codigo_subacao = "sa_pai_" . $entid;
		?>
			arvore.add( '<?= $codigo_subacao ?>', '<?= $codigo ?>', "Sub-ações (<?= count( $subacaoes ) ?>)", 'javascript:void(0);' );
		<?php foreach ( $subacaoes as $subacao ): ?>
			arvore.add( 'sa_<?= $subacao["sbaid"] ?>', '<?= $codigo_subacao ?>', "<?= $subacao["sbadsc"] ?>", "javascript:alterarSubacao( '<?= $subacao["sbaid"] ?>' );" );
		<?php endforeach;
    }?>
	elemento = document.getElementById( '_arvore' );
	elemento.innerHTML = arvore;
--></script>
<p align="center">
	<input
		type="button"
		name="Adicionar / Remover"
		value="Adicionar / Remover"
		onclick="escolherEscolas();"
	/>
	&nbsp;&nbsp;&nbsp;
	<input
		type="button"
		name="Voltar"
		value="Voltar"
		align="right"
		onclick="voltar();"
	/>
</p>


<? /*
<script type="text/javascript">
	
	function escolherEscolas()
	{
		window.open( '?modulo=principal/escolasescolha&acao=A','blank','height=400,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	}
	
	function voltar()
	{
		window.location.href = '?modulo=principal/estrutura_avaliacao&acao=A';
	}
	
</script>
<table align="center" class="tabela" border="0" cellpadding="3" cellspacing="1">
	<?php $cor = ""; ?>
	<?php if ( count( $escolas ) ) : ?>
		<?php foreach ( $escolas as $escola ) : ?>
			<?php $cor = $cor == "#f0f0f0" ? "#e0e0e0" : "#f0f0f0"; ?>
			<tr bgcolor="<?= $cor ?>">
				<td>
					<a href="?modulo=principal/escola&acao=A&entid=<?= $escola['entid'] ?>">
						<?= $escola['entnome'] ?>
					</a>
				</td>
			</tr>
		<?php endforeach; ?>
	<?php else: ?>
		<tr bgcolor="#efefef">
			<td style="color:#dd2020;text-align:center;">
				<br/><br/>
				Não há escolas selecionadas
				<br/><br/>
			</td>
		</tr>
	<?php endif; ?>
	<tr style="background-color: #dfdfdf; border: 1px solid #cccccc">
		<td>
			<input
				type="button"
				name="Adicionar"
				value="Adicionar"
				onclick="escolherEscolas();"
			/>
			<input
				type="button"
				name="Voltar"
				value="Voltar"
				align="right"
				onclick="voltar();"
			/>
		</td>
	</tr>
</table>
*/
?>