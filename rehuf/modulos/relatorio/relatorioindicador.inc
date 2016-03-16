<?php
if ( isset( $_REQUEST['buscar'] ) ) {
	include APPRAIZ ."www/rehuf/_funcoesindicadores.php";
	include "relatorioindicador_resultado.inc";
	exit;
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório Indicadores - REHUF", "" );

?>
<script type="text/javascript">
function exibirRelatorio() {
	var formulario = document.formulario;
	// verifica se tem algum agrupador selecionado
	agrupador = document.getElementById( 'agrupador' );
	if ( !agrupador.options.length ) {
		alert( 'Escolha ao menos um item para agrupar o resultado.' );
		return;
	}
	selectAllOptions( agrupador );
	selectAllOptions( document.getElementById( 'entid' ) );

	// submete formulario
	formulario.target = 'relatoriogeralrehuf';
	var janela = window.open( '', 'relatoriogeralrehuf', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
}
</script>

<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Indicadores :</td>
			<td width="80%">
			<?
$agrupadorHtml =
<<<EOF
    <table>
        <tr valign="middle">
            <td>
                <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 250px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <!--
                <img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
                -->
                <img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
            </td>
            <td>
                <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 250px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
            </td>
        </tr>
    </table>
    <script type="text/javascript" language="javascript">
        limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
        limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
        {POVOAR_ORIGEM}
        {POVOAR_DESTINO}
        sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
    </script>
EOF;
					// inicia agrupador
					$agrupador = new Agrupador( 'formulario', $agrupadorHtml );
					$destino = array();
					$sql = "SELECT indid AS codigo, inddsc AS descricao FROM rehuf.indicador";
					$tabelas = $db->carregar($sql);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $tabelas );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
					
				?>
			</td></tr>
        <tr><td>Filtros</td></tr>		
		<tr>
			<td class="SubTituloDireita" valign="top">Hospitais :</td>
			<td>
			<?
			$permissoes = verificaPerfilRehuf();
			if(count($permissoes['verhospitais']) > 0) {
				$sqlHospitais = "SELECT ent.entid as codigo, ent.entnome||' - '||coalesce(ena.entsig, '') as descricao FROM entidade.entidade ent 
								 LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid  
								 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
								 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid  
								 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
								 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) AND ent.entid IN('".implode("','",$permissoes['verhospitais'])."') 
								 ORDER BY ena.entsig";
				combo_popup( "entid", $sqlHospitais, "Hospitais", "192x400", 0, array(), "", "S", false, false, 5, 400 );				
				
			} else {
				echo "Seu perfil não possui hospitais";	
			}
			?>
			</td>
		</tr>
	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;"><input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();"/></td>
	</tr>
</table>