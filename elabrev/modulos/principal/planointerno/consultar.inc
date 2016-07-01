<?php
/* Deleção do Plano Interno
*/
if ($_GET[pliidDel]):
/*
* Verifica se o PI está vinculado há alguma atividade
*/
	$sql = sprintf("SELECT COUNT(pliid)
					FROM pde.planointernoatividade
					WHERE pliid = %d", $_GET[pliidDel]);
	$register = $db->pegaUm($sql);
	unset($sql);
/*
* Deleta o Plano Interno
*/
	if (!$register):
		$sql = sprintf("DELETE FROM financeiro.planointerno
						WHERE pliid = %d", $_GET[pliidDel]);
		$db->executar($sql);
		$db->commit();
	
		die('<script>
				alert(\'Operação executada com sucesso!\');
				location.href=\'?modulo=principal/planointerno/consultar&acao=A&uo='.$_GET[uo].'\';
			 </script>');
	endif;
		die('<script>
				alert(\'Operação não realizada!\nEste Plano Interno está vinculado a uma atividade.\');
				location.href=\'?modulo=principal/planointerno/consultar&acao=A&uo='.$_GET[uo].'\';
			 </script>');
endif;
	
include APPRAIZ . "includes/cabecalho.inc";

?>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<?php
print '<br/>';
monta_titulo( 'Manutenção do Plano Interno', '<img src=\'/imagens/gif_inclui.gif\'/> Incluir | <img src=\'/imagens/alterar.gif\' /> Alterar | <img src=\'/imagens/excluir.gif\' /> Excluir'  );
?>
<form action="?modulo=principal/planointerno/consultar&acao=A" method="post" name="formPesq">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<TR>
			<TD class="SubTituloDireita">Unidade Orçamentária</TD>
			<TD>
			<?php
				$sql ="SELECT
						 DISTINCT uni.unicod AS codigo, 
						 uni.unicod || ' - ' || uni.unidsc AS descricao
					   FROM
					     public.unidade uni
			 			 JOIN monitora.acao  aca on uni.unicod = aca.unicod 
			 			 	and uni.unitpocod = 'U'
					   WHERE 
					     aca.prgano = '" . $_SESSION['exercicio'] . "' AND
					     aca.acasnrap = false
					   GROUP BY
					     uni.unicod, 
					     uni.unidsc
					   ORDER BY
					     descricao";
			$uo = $_POST[uo] ? $_POST[uo] : $_GET[uo];	
			$db->monta_combo('uo',$sql,'S','Escolha uma Unidade Orçamentária','document.formPesq.submit();','');
			unset($sql);	
			?>
			</TD>
		</TR>
</form>		
		<?php
		if ($uo):
			$sql ="SELECT
					 DISTINCT uni.unicod AS codigo, 
					 uni.unicod || ' - ' || uni.unidsc AS descricao
				   FROM
				     public.unidade uni
		 			 JOIN monitora.acao  aca on uni.unicod = aca.unicod 
		 			 	and uni.unitpocod = 'U'
				   WHERE 
				     aca.prgano = '" . $_SESSION['exercicio'] . "' AND
				     aca.acasnrap = false AND
				     uni.unicod = '". $uo ."'
				   GROUP BY
				     uni.unicod, 
				     uni.unidsc
				   ORDER BY
				     descricao";
				
			$unidade = $db->pegaLinha($sql);	
			unset($sql);
			
			# Pegar Ações	
			$sql = "SELECT
					 DISTINCT ac.acaid, 
					 ac.unicod AS codigo,
					 ac.prgcod || '.' || ac.acacod || '.' || ac.unicod || '.' || ac.loccod || ' - ' || ac.acadsc AS descricao 
					FROM
					 public.unidade u
					 JOIN monitora.acao ac ON ac.unicod = u.unicod
					 	and u.unitpocod = 'U' 
					WHERE
 					 ac.prgano = '" . $_SESSION['exercicio'] . "' AND 
 					 ac.acasnrap = false AND
 					 ac.unicod = '". $uo ."'
					GROUP BY
					 ac.acaid,
					 ac.unicod,
					 ac.prgcod,
					 ac.acacod,
					 ac.unicod,
					 ac.loccod,
					 ac.acadsc";
			$dadoAcao = $db->carregar($sql);
			unset($sql);

			# Pegar Plano Interno
			$sql = "SELECT
			 		 pi.acaid,
			   		 pliid,
			   		 plicod,
			   		 tpcid as tipoconvencao,
			   		 tpmid as tipomodalidade,
					 plicod || ' - ' || plidsc as descricao
					FROM
					 monitora.acao ac
					 JOIN financeiro.planointerno pi ON pi.acaid = ac.acaid
					WHERE
					 ac.prgano = '" . $_SESSION['exercicio'] . "' AND 
 					 ac.acasnrap = false AND
 					 ac.unicod = '". $uo ."'
					GROUP BY
					 pliid,
					 plicod,
					 tpcid,
					 tpmid,
					 descricao,
					 pi.acaid";

			$dadoPi = $db->carregar($sql);
			unset($sql);	
		?>
		<tr>
		<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
			<div id="bloco" style="overflow: hidden;">
				<p>
				<a href="javascript: arvore.openAll();">Abrir Todos</a> &nbsp;|&nbsp;
				<a href="javascript: arvore.closeAll();">Fechar Todos</a>
				</p>
				<div id="_arvore"></div>
			</div>
			<script type="text/javascript">
					arvore = new dTree( "arvore" );
				arvore.config.folderLinks 	= true;
				arvore.config.useIcons 		= true;
				arvore.config.useCookies 	= true; 
				arvore.add( 0, -1, 'Plano Interno');
				<? 
				echo "arvore.add('u_{$unidade[codigo]}',0,'{$unidade[descricao]}','');\n";
			
				# Monta árvore Ações 
				if (is_array($dadoAcao)):
					 foreach ($dadoAcao as $acaoRec){
					 	$img = "<img src=\"/imagens/gif_inclui.gif\" title=\"Cadastrar Plano Interno\" onclick=\"abrepopup('{$acaoRec[acaid]}',1)\" style=\"cursor:pointer;\">";
				 		echo "arvore.add('a_{$acaoRec[acaid]}','u_{$acaoRec[codigo]}','". str_replace("'", "\'", $img.' '.$acaoRec[descricao])."','javascript:void(0);');\n";
					 }
					 unset($img);	
				endif;		
			
				# Monta árvore Plano Interno
				if (is_array($dadoPi)):
					 foreach ($dadoPi as $piRec){
					 	$img = "<img src=\"/imagens/alterar.gif\" title=\"Editar Plano Interno\" onclick=\"abrepopup('{$piRec[pliid]}',2)\" style=\"cursor:pointer;\">";
					 	if( ($piRec['tipoconvencao'] && $piRec['tipomodalidade'] ) ){
					 	$img .= "&nbsp;&nbsp;<img src=\"/imagens/excluir.gif\" title=\"Deletar Plano Interno\" onclick=\"delPi('{$piRec[pliid]}','{$piRec[descricao]}',{$uo});\" style=\"cursor:pointer;\">"; 
					 	}
					 	
				 		echo "arvore.add('p_{$piRec[pliid]}','a_{$piRec[acaid]}','". str_replace("'", "\'", $img.' '.$piRec[descricao])."');\n";
					 }
				endif;		
				?>
				elemento = document.getElementById( '_arvore' );
				elemento.innerHTML = arvore;
		
				 /*
				 * adaptação de largura e controle de overflow de conteúdo para o internet explorer
				 */
				var dav = navigator.appVersion;
				IE = document.all ? true : false;
				IE6 = dav.indexOf( "MSIE 6.0" ) >= 0;
				IE7 = dav.indexOf( "MSIE 7.0" ) >= 0;
				if ( IE ) {
					width = document.body.offsetWidth;
					height = document.body.offsetHeight;
					document.getElementById( 'bloco' ).style.width = ( width * 0.80 ) + 'px';
				}			
			</script>
		</td>
		<td style="background-color:#fafafa; color:#404040; width: 1px;" valign="top"></td>
		</tr>
		 <?php
		 endif;
		 ?>
	</tbody>
</table>

<script type="text/javascript">
function delPi(pliid, plidsc,uo) {
	if (confirm('Deseja apagar o Plano Interno "'+plidsc+'" ?')) {
		location.href = window.location+'&pliidDel='+pliid+'&uo='+uo;
		return true;
	}
	return false;
}

function abrepopup(valor, param){
	if (param==1)
		urlParam = '?acaid='+ valor;
	else
		urlParam = '?pliid='+ valor;
	
	janela = window.open('geral/cadplanointerno.php'+urlParam,'Cadastro','width=800,height=290,scrollbars=1');
	janela.focus();
}
</script>
