<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

if( $_REQUEST["req"] == "aderirEscolaAtiva" ){
	
	$obEscolaAtiva = new EscolaAtiva();
	$obEscolaAtiva->aderirEscolaAtiva();
	
	unset( $_REQUEST["req"] );
	echo "Operação Efetuada com sucesso!";
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );

?>

<script src="../includes/prototype.js"></script>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="post" name="formulario">
					<input type="hidden" name="modulo" value="<?= $_REQUEST['modulo'] ?>"/>
					<div style="float: left;">
						
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									Município
									<br/>
									<?php $filtro = simec_htmlentities( $_REQUEST['filtro'] ); ?>
									<?= campo_texto( 'mundescricao', 'N', 'S', '', 50, 200, '', '', '', '', '', '', '', $_REQUEST["mundescricao"] ); ?>
								</td>
								<td valign="bottom">
									Estado
									<br/>
									<?php
									$estuf = $_REQUEST['estuf'];
									$unidades = "'" . implode( "','", cte_pegarUfsPermitidas() ) . "'";
									$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e where e.estuf in ({$unidades}) order by e.estdescricao asc";
									$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
									?>
								</td>
							</tr>
							<tr>
								<td>
									Situação<br/>
									<?php $sql = "	select 99 as codigo, 'Não Iniciado' as descricao
													union
													select esdid as codigo, esddsc as descricao 
													from workflow.estadodocumento where tpdid = 7";
									$db->monta_combo( "esdid", $sql , 'S', 'Todas as Situações', '', '', '', '', '', '', '', $_REQUEST["esdid"] ); ?>
								</td>
								<td>
									Liberado<br/>
									<?php 
									
									$arLiberado[0]["codigo"] = "1";
									$arLiberado[0]["descricao"] = "Sim";
									
									$arLiberado[1]["codigo"] = "2";
									$arLiberado[1]["descricao"] = "Não";
									
									$db->monta_combo( "boLiberado", $arLiberado , 'S', 'Todos', '', '', '', '', '', '', '', $_REQUEST["boliberado"] ); ?>
								</td>
							</tr>
						</table>
					</div>
					<div style="float: right;">
						<input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
					</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>
<?php

$arWhere = array();
if( $_REQUEST["estuf"] ){
	$arWhere[] = " m.estuf = '{$_REQUEST["estuf"]}' ";
}
if( $_REQUEST["mundescricao"] ){
	$arWhere[] = " m.mundescricao ilike '%{$_REQUEST["mundescricao"]}%' ";
}
if( $_REQUEST["esdid"] ){
	$arWhere[] =  $_REQUEST["esdid"] == 99 ? " ed.esdid is null " : " ed.esdid = '{$_REQUEST["esdid"]}' ";
}
if( $_REQUEST["boliberado"] ){
	$condicao = $_REQUEST["boliberado"] == 1 ? " = 1 " : " != 1 ";  
	$arWhere[] = " coalesce( iu.inusituacaoadesao, 0 ) $condicao ";
}

$arWhere = count( $arWhere ) ? " and ". implode( " and ", $arWhere ) : "";

$sql = "select 	distinct 
			case
				when ( coalesce( iu.inusituacaoadesao, 0 ) = 1 ) then '<img onclick=\"excluirEscolaAtiva( '|| iu.inuid ||' )\" src=\"/imagens/excluir.gif\" border=0 title=\"Excluir Escola Ativa\"></a>'
				else '<img onclick=\"aderirEscolaAtiva( '|| iu.inuid ||', \''|| coalesce( iu.inusituacaoadesao, 0 ) ||'\' )\" src=\"/imagens/alterar.gif\" border=0 title=\"Aderir Escola Ativa\"></a>'
			end as acao,
			iu.mun_estuf, m.mundescricao, 
			case
				when ( coalesce( iu.inusituacaoadesao, 0 ) = 1 ) then 'Sim'
				else 'Não'
			end as liberado,
			coalesce( ed.esddsc,'Não iniciado' ) as esddsc,
			case
				when ( coalesce( iu.inusituacaoadesao, 0 ) = 1 ) then 'Adesão Confirmada'
				when ( coalesce( iu.inusituacaoadesao, 0 ) = 2 ) then 'Não Confirmou a Adesão'
				when ( coalesce( iu.inusituacaoadesao, 0 ) = 3 ) then 'Não Há Classes Multisseriadas'
				else 'Não se Manifestou'
			end as situacaoadesao
		from cte.instrumentounidade as iu
			inner join territorios.municipio m on m.muncod = iu.muncod
			left join cte.escolaativa ea on ea.inuid = iu.inuid
			left join workflow.documento d on d.docid = ea.docid
			left join workflow.estadodocumento ed on ed.esdid = d.esdid
			inner join workflow.documento doc on doc.docid = iu.docid
		where iu.itrid = 2 
		and doc.esdid != 1
		$arWhere 
		order by iu.mun_estuf, m.mundescricao";

$db->monta_lista( $sql, array( "Ação", "UF", "Município", "Liberado Escola Ativa", "Situação Escola Ativa", "Situação da Adesão" ), 30, 10, 'N', '', '' ); 
?>

<script type="text/javascript">
	function pesquisar(){
		formulario.submit();
	}
	
	function excluirEscolaAtiva( inuid ){
 
		if( confirm( "Deseja realmente excluir todos os dados referentes ao Programa Escola Ativa deste Município, bem como cancelar sua adesão ao programa?" ) ){
			
			return windowOpen('/cte/cte.php?modulo=principal/escolaAtivaExclusao&acao=A&inuid=' + inuid, 'escolaAtivaExclusao',
	                          'height=200,width=300,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes'); 			
			
		}
	}
	
	function aderirEscolaAtiva( inuid, situacao ){
		
		switch( situacao ){
			case( '2' ): var texto = "O Município em questão havia selecionado a opção 'Não Confirmo a Adesão'.\nDeseja realmente Aderir ao Programa? "; break; 
			case( '3' ): var texto = "O Município em questão havia selecionado a opção 'Não há Classes Multisseriadas'.\nDeseja realmente Aderir ao Programa? "; break; 
			default: var texto = "O Município não havia se manifestado em relação ao Programa Escola Ativa.\nDeseja realmente Aderir ao Programa? "; break; 
		}
		
		if( confirm( texto ) ){
	        return new Ajax.Request(window.location.href, {
				method: 'post',
				parameters: '&req=aderirEscolaAtiva&inuid=' + inuid,
				asynchronous: false,
				onComplete: function( res ){	
					alert( res.responseText );
					window.location.reload();
				}
			});
		}
	}
	
</script>