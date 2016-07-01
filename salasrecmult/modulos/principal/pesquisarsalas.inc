<?
 /*
   Sistema Simec
   Setor responsável: MEC
   Desenvolvedor: Equipe Desenvolvedores Simec
   Analista: Vitor Nunes Sad (vitor.sad@mec.gov.br)
   Programador: Fernando Rolim (fernandosilvabsb@hotmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir pesquisa de salas de recursos multifuncionais
    */

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

if(isset($_REQUEST['requisicao']) && $_REQUEST['requisicao'] == 'excluir') {
	$salid = $_REQUEST['salid']; 
	$sql = "UPDATE salasrecmult.salas SET
			   salstatus = 'I'   
			WHERE salid=$salid";
	$db->executar($sql);
	$db->commit();	
}

?>
	<br/>
	<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
	<script type="text/javascript" src="../includes/remedial.js"></script>
	<script type="text/javascript" src="../includes/superTitle.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">

	<script type="text/javascript">
		function alterarSala(salid) {
			window.location.href = "salasrecmult.php?modulo=principal/cadsala&acao=A&salid=" + salid;	
		}

		function excluirSala(salid) {
			if (confirm ('Deseja realmente excluir esta sala?')){
				window.location.href = "salasrecmult.php?modulo=principal/pesquisarsalas&acao=A&requisicao=excluir&salid=" + salid;
				alert ('sala excluída com sucesso.');
			}
		}
			
	</script>	

	<?php
		$db->cria_aba( $abacod_tela, $url, '' );
		$titulo_modulo = "Pesquisa de Salas de Recursos Multifuncionais";
		monta_titulo( $titulo_modulo, '<div style="margin:3px;" align="right" class="' . $class . '"></div>' );
	?>
	
	<form method="post" name="filtro">
		<input type="hidden" name="filtro" value="1"/>
		<!-- <table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;"> -->
		<table align="center" width="95%" cellSpacing="1" cellPadding="3" class="tabela" bgcolor="#f5f5f5" >
	      <tr>
	        <td align='right' class="SubTituloDireita">C&oacute;digo da sala:</td>
	        <td><?=campo_texto('salcodigosala','N','S','',13,10,'','');?></td>
	      </tr>			
	      <tr>
	        <td align='right' class="SubTituloDireita">Esfera:</td>
	        <td>
				<?
				$sql2 = "select esfid as CODIGO, esfdescricao as DESCRICAO from salasrecmult.esferas order by DESCRICAO ";
				$db->monta_combo("esfid", $sql2, 'S' , 'Escolha uma esfera','','Todas as esferas');
				?>	        
	        </td>
	      </tr>			

	      <tr>
	        <td align='right' class="SubTituloDireita">Tipo:</td>
	        <td>
				<?
				$sql2 = "select tipid as CODIGO, tipdescricao as DESCRICAO from salasrecmult.tipos order by tipid ";
				$db->monta_combo("tipid",$sql2,'S',"Escolha um tipo",'','Todos os tipos');
				?>	        
	        </td>
	      </tr>			
	      <tr>
	        <td align='right' class="SubTituloDireita">INEP:</td>
	        <td><?=campo_texto('entcodent','N','S','',13,10,'','');?></td>
	      </tr>			
	      
	      <tr>
	        <td align='right' class="SubTituloDireita">Situa&ccedil;&atilde;o:</td>
	        <td>
				<?
				$sql2 = "select sitid as CODIGO, sitdescricao as DESCRICAO from salasrecmult.situacoes order by sitid ";
				$db->monta_combo("sitid",$sql2,'S',"Escolha uma situação",'','Todas as situações');
				?>	        
	        </td>
	      </tr>
   	      <tr>
	        <td align='right' class="SubTituloDireita">Transferidos em (ano):</td>
	        <td>
				<?
				$sql2 = "select distinct date_part('Y', tardatatransferencia) as CODIGO, date_part('Y', tardatatransferencia) as DESCRICAO from salasrecmult.transferencias order by 1 desc";
				$db->monta_combo("tardatatransferencia_ano",$sql2,'S',"Escolha o ano de transferência",'','');
				?>	        
	        </td>
	      </tr>

   	      <tr>	      
			<td class="SubTituloDireita" style="text-align:center;">
				&nbsp;
			</td>
			<td class="SubTituloDireita" style="text-align:left;">
				<input type="Button" style="width:80px;" name="Filtrar" value="Filtrar" onclick="document.filtro.submit();">
			</td>
	      </tr>
		</table>
	</form>

	<table align="center" width="95%" border="0" cellpadding="2" cellspacing="0" class="listagem" style="color:#333333">
		<tbody>
			<?
//			salasrecmult.php?modulo=principal/cadsala&acao=A			
			
			$sql_lista = "
				select 
 				   '<img 	src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"  
							id=\"img ' ||  salas.salid || '\" name=\"+\" 
							onclick=\"javascript:alterarSala(' || salas.salid || ');\"/>'
	
					|| '&nbsp;'
							
 				   '<img 	src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"  
							id=\"img ' ||  salas.salid || '\" name=\"+\" 
							onclick=\"javascript:excluirSala(' || salas.salid || ');\"/>'							
							
							as acoes,
				   salas.salcodigosala    as codigo_sala, 
				   bentidade.entnome      as nome_escola,
				   esferas.esfdescricao   as esfera, 
				   tipos.tipdescricao     as tipo, 
				   situacoes.sitdescricao as situacao,
				   'ENTREGUE'              as entrega, 
				   case salas.salid in (select btransf.salid from salasrecmult.transferencias btransf) 
				     when true then '<span style=\"color:#0066cc;\">Sim</span>'
				     else 'Não' 
				   end as transferido
				from 
				   salasrecmult.esferas as esferas
				   
				   inner join salasrecmult.salas as salas 
				   on esferas.esfid = salas.esfid
				
				   inner join salasrecmult.tipos as tipos
				   on salas.tipid = tipos.tipid
				
				   inner join salasrecmult.situacoes as situacoes
				   on salas.sitid = situacoes.sitid   
				   
				   inner join entidade.entidade bentidade
				   on salas.entid = bentidade.entid
				where 
				   salas.salstatus = 'A' and "; 
			
			$sql_lista .= add_clausula_select('salcodigosala','salas.salcodigosala');
			
			$sql_lista .= add_clausula_select('esfid','esferas.esfid', false);

			$sql_lista .= add_clausula_select('tipid','tipos.tipid', false);
			
			$sql_lista .= add_clausula_select('entcodent','bentidade.entcodent', false);

			$sql_lista .= add_clausula_select('sitid','situacoes.sitid', false);
			
			$tardatatransferencia_ano = $_REQUEST['tardatatransferencia_ano'];
			if ( $tardatatransferencia_ano != "" && isset($tardatatransferencia_ano) ) {
				$sql_lista .= " salas.salid in (select salid from salasrecmult.transferencias ctransf where date_part('Y', tardatatransferencia) = '$tardatatransferencia_ano') and "; 
			}

			$sql_lista .= " 0=0 "; 
			
			$cabecalho = array ("", "Código da sala", "Nome da escola", "Esfera", "Tipo", "Situação", "Entrega", "Transferida" );
			$db->monta_lista($sql_lista,$cabecalho,60,20,'','','');			
			?>
		</tbody>
	</table>	

<?
function is_combo_set($campo='') {
	$auxiliar = $_REQUEST[$campo];
	return isset($auxiliar) && $auxiliar != "" && $auxiliar != "x";
} 

function add_clausula_select($campo_formulario,$campo_base_dados,$is_string=true) {
	if ( is_combo_set($campo_formulario) ) {
		$parametro = $_REQUEST[$campo_formulario];
		return " $campo_base_dados = " . ($is_string ? "'":"") . $parametro . ($is_string ? "'":"") . " and ";
	} else {
		return " ";
	}
}
?>