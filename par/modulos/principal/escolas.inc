<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Escolas Atendidas', ''  );

$inuid = (integer) $_SESSION["par"]["inuid"];
$itrid = pegarItrid( $inuid );

if($_SESSION['par']['muncod']){
	/*
	$sql = "SELECT 
				e.entid, 
				upper(e.entnome) as entnome, 
				mu.muncod, 
				mu.mundescricao
			FROM
				".SCHEMAEDUCACENSO.".tab_entidade ent 
			INNER JOIN ".SCHEMAEDUCACENSO.".tab_dado_escola ted on ted.fk_cod_entidade= ent.pk_cod_entidade
			INNER JOIN entidade.entidade e on e.entcodent = ent.pk_cod_entidade::character varying
			INNER JOIN entidade.endereco 	    ed ON ed.entid = e.entid
			INNER JOIN territorios.municipio    mu ON mu.muncod = ed.muncod
			WHERE 
				(e.entescolanova = false OR e.entescolanova IS NULL)  
				AND e.entstatus = 'A' 
				AND e.tpcid = 3 
				AND e.entcodent IS NOT NULL
				AND ed.muncod = '".$_SESSION['par']['muncod']."'
			GROUP BY 
				e.entid, e.entnome, mu.muncod, mu.mundescricao
			ORDER BY 
				mu.mundescricao, e.entnome";
	*/
	$sql = "
			SELECT distinct 
				 e.entid, 
				upper(e.entnome) as entnome  , 
				 mu.muncod, 
				 mu.mundescricao
			FROM
				".SCHEMAEDUCACENSO.".tab_entidade 		ent 
			INNER JOIN ".SCHEMAEDUCACENSO.".tab_dado_escola 	ted on ted.fk_cod_entidade= ent.pk_cod_entidade
			INNER JOIN ".SCHEMAEDUCACENSO.".tab_municipio m on m.pk_cod_municipio = ent.fk_cod_municipio
			INNER JOIN entidade.entidade 			e on e.entcodent = ent.pk_cod_entidade::character varying and e.entstatus = 'A'
			INNER JOIN territorios.municipio    		mu ON mu.muncod::character  = ent.fk_cod_municipio::character
			WHERE 
				(e.entescolanova = false OR e.entescolanova IS NULL)  
				 AND e.entstatus = 'A' 
				 AND e.tpcid = 3 
				 AND e.entcodent IS NOT NULL
				AND m.pk_cod_municipio = '".$_SESSION['par']['muncod']."'
				AND mu.muncod = '".$_SESSION['par']['muncod']."'
			 GROUP BY e.entid, e.entnome, mu.muncod, mu.mundescricao
			 ORDER BY mu.mundescricao, entnome;
	";

}else{
	$sql = "SELECT 
				e.entid, 
				upper(e.entnome) as entnome, 
				mu.muncod, 
				mu.mundescricao
			FROM ".SCHEMAEDUCACENSO."
				.tab_entidade ent 
			INNER JOIN ".SCHEMAEDUCACENSO.".tab_dado_escola ted on ted.fk_cod_entidade= ent.pk_cod_entidade
			INNER JOIN entidade.entidade e on e.entcodent = ent.pk_cod_entidade::character varying
			INNER JOIN entidade.endereco 		ed ON ed.entid = e.entid
			INNER JOIN territorios.municipio 	mu ON mu.muncod = ed.muncod
			WHERE 
				(e.entescolanova = false OR e.entescolanova IS NULL)  
				AND e.entstatus = 'A' 
				AND e.tpcid = 1  
				AND e.entcodent IS NOT NULL
				AND ed.estuf = '".$_SESSION['par']['estuf']."'
			GROUP BY 
				e.entid, e.entnome, mu.muncod, mu.mundescricao
			ORDER BY 
				mu.mundescricao, e.entnome";
}

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
		window.location.href = '?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore';
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
//			$nome = '<a href=\\"?modulo=principal/escolas&acao=A&entid=' . $entid . '\\">' . $entnome . '</a>';
			$nome = '<a href=\\"\\">' . $entnome . '</a>';
			$codigo = 'es_' . $entid;
		?>
			arvore.add( '<?= $codigo ?>', '<?= $codigo_pai ?>', "<?= $nome ?>", 'javascript:void(0);' );
        <?php
		$sql = "SELECT
					sa.sbaid,
					sa.sbadsc
				FROM 
					cte.qtdfisicoano qfa
				INNER JOIN par.subacao 	 sa ON sa.sbaid = qfa.sbaid AND sa.sbastatus = 'A'
				INNER JOIN par.acao 	 ac ON ac.aciid = sa.aciid AND ac.acistatus = 'A'
				INNER JOIN par.pontuacao po ON po.ptoid = ac.ptoid AND po.ptostatus = 'A'
				WHERE
					qfa.entid = ".$entid." AND
					po.inuid = ".$inuid."
				GROUP BY
					sa.sbaid,
					sa.sbadsc";
// Back-up sql antiga		
//			select
//				sa.sbaid,
//				sa.sbadsc
//			from cte.qtdfisicoano qfa
//				inner join cte.subacaoindicador sa on
//					sa.sbaid = qfa.sbaid
//				inner join cte.acaoindicador ac on
//					ac.aciid = sa.aciid
//				inner join cte.pontuacao po on
//					po.ptoid = ac.ptoid
//			where
//				qfa.entid = " . $entid . " and
//				po.ptostatus = 'A' and
//				po.inuid = " . $inuid . "
//			group by
//				sa.sbaid,
//				sa.sbadsc";
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
<?php
	$perfil = pegaPerfilGeral();
	if(	(in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)	)	){
?>
	<input type="button" name="Adicionar / Remover" value="Adicionar / Remover" onclick="escolherEscolas();" />
	&nbsp;&nbsp;&nbsp;
<?php }	?>
	<input type="button" name="Voltar" value="Voltar" align="right" onclick="voltar();" />
</p>
