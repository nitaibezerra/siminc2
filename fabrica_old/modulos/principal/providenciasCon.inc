<?php

if($_REQUEST['terid']){
	echo $db->pegaUm("select corpo from fabrica.termo where terid = ".$_REQUEST['terid']);
	die;
}

include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

if( ($_REQUEST['tipoaba'] == 'analiseDemanda') ){
	
	$menu = array(0 => array("id" => 1, "descricao" => "Analisar Solicitação", 	"link" => "/fabrica/fabrica.php?modulo=principal/analiseDemandaCon&acao=A&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 		 	"link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacaoCon&acao=A&tipoobs=analiseDemanda"),
				  2 => array("id" => 3, "descricao" => "Anexos da Solicitação",	"link" => "/fabrica/fabrica.php?modulo=principal/analiseDemandaAnexosCon&acao=".$_REQUEST['acao']),
				  3 => array("id" => 4, "descricao" => "Providências", 		 	"link" => "/fabrica/fabrica.php?modulo=principal/providenciasCon&acao=A&tipoaba=analiseDemanda")
			  	  );
}
elseif( ($_REQUEST['tipoaba'] == 'cadAvaliacaoAprovacao') ){
 	$menu = array(0 => array("id" => 1, "descricao" => "Avaliar / Aprovar Solicitação",      "link" => "/fabrica/fabrica.php?modulo=principal/cadAvaliacaoAprovacaoCon&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 						 "link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacaoCon&acao=A&tipoobs=cadAvaliacaoAprovacao"),
				  2 => array("id" => 3, "descricao" => "Anexos da ordem de serviço",         "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexosCon&acao=C"),
				  3 => array("id" => 4, "descricao" => "Providências", 		 		 		 "link" => "/fabrica/fabrica.php?modulo=principal/providenciasCon&acao=A&tipoaba=cadAvaliacaoAprovacao")
			  	  );
}
elseif( ($_REQUEST['tipoaba'] == 'cadDetalhamento') ){
 	$menu = array(0 => array("id" => 1, "descricao" => "Detalhar Solicitação",		 "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoCon&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 				 "link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacaoCon&acao=A&tipoobs=cadDetalhamento"),
				  2 => array("id" => 3, "descricao" => "Anexos da Solicitação",		 "link" => "/fabrica/fabrica.php?modulo=principal/analiseDemandaAnexosCon&acao=C"),
				  3 => array("id" => 4, "descricao" => "Anexos da Ordem de Serviço", "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexosCon&acao=A"),
				  4 => array("id" => 5, "descricao" => "Providências", 		 		 "link" => "/fabrica/fabrica.php?modulo=principal/providenciasCon&acao=A&tipoaba=cadDetalhamento")
			  	  );
}
elseif( ($_REQUEST['tipoaba'] == 'cadExecucao') ){
 		$menu = array(0 => array("id" => 1, "descricao" => "Executar Solicitação",      		 "link" => "/fabrica/fabrica.php?modulo=principal/cadExecucaoCon&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 						 "link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacaoCon&acao=A&tipoobs=cadExecucao"),
				  2 => array("id" => 3, "descricao" => "Anexos da ordem de serviço",         "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexosCon&acao=D"),
				  3 => array("id" => 4, "descricao" => "Monitoramento / Controle de riscos", "link" => "/fabrica/fabrica.php?modulo=principal/monitoramentoRiscosCon&acao=C"),
				  4 => array("id" => 5, "descricao" => "Providências", 		 		 	 	 "link" => "/fabrica/fabrica.php?modulo=principal/providenciasCon&acao=A&tipoaba=cadExecucao")
			  	  );
}

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);


	


$titulo_modulo = "Providências";
monta_titulo( $titulo_modulo, "&nbsp;");

    


?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<form method="post" name="formulario" id="formulario">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" border="0">
		<tr>
			<td>
				 <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" border="0">
					<tr>
						<td class="TituloTela" bgcolor="#DCDCDC">
							<b>Solicitação de Serviços (SS)</b>
						</td>
					</tr>
					<tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
						<td>
							<?php 
								$sql = "select 
											'<center><img src=\"/imagens/consultar.gif\" border=0 title=\"Visualizar\" style=\"cursor:pointer;\" onclick=\"window.open(\'fabrica.php?modulo=principal/providencias&acao=A&terid='||t.terid||'\',\'termo\', \'height=600, width=780, scrollbars=1\');\"></center>' as acao,
											tt.descricao as tipo,
											'<center>'||t.scsid||'</center>' as ss,
											'<center>'||u.usunome||'</center>' as nome,
											'<center>'||to_char(t.data::timestamp,'DD/MM/YYYY HH24:MI:SS')||'</center>' as dataemissao
										from fabrica.termo t
										inner join fabrica.tipotermo tt on tt.tptid = t.tptid
										inner join seguranca.usuario u on u.usucpf = t.usucpf
										inner join (select max(terid) as terid, scsid, tptid from fabrica.termo group by scsid,tptid) t2 on t2.terid=t.terid and t2.tptid = t.tptid
										where  t.odsid is null
										and t.scsid = ".$_SESSION["fabrica_var"]["scsid"];
								//dbg($sql,1);
								$cabecalho = array("Ação","Tipo de Termo","Nº SS","Emitido por","Data Emissão");
								$db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'N', '100%', 'N' );
							?>
						</td>
					</tr>					
				 </table>
				 <br>
				 <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" border="0">
					<tr>
						<td class="TituloTela" bgcolor="#DCDCDC">
							<b>Ordem de Serviços (OS Genérica)</b>
						</td>
					</tr>
					<tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
						<td>
							<?php 
								$sql = "select 
											'<center><img src=\"/imagens/consultar.gif\" border=0 title=\"Visualizar\" style=\"cursor:pointer;\" onclick=\"window.open(\'fabrica.php?modulo=principal/providencias&acao=A&terid='||t.terid||'\',\'termo\', \'height=600, width=780, scrollbars=1\');\"></center>' as acao,
											tt.descricao as tipo,
											'<center>'||t.odsid||'</center>' as os,
											'<center>'||u.usunome||'</center>' as nome,
											'<center>'||to_char(t.data::timestamp,'DD/MM/YYYY HH24:MI:SS')||'</center>' as dataemissao
										from fabrica.termo t
										inner join fabrica.tipotermo tt on tt.tptid = t.tptid
										inner join fabrica.ordemservico os on os.odsid = t.odsid
										inner join seguranca.usuario u on u.usucpf = t.usucpf
										inner join (select max(terid) as terid, odsid, tptid from fabrica.termo group by odsid,tptid) t2 on t2.terid=t.terid and t2.tptid = t.tptid
										where  t.odsid is not null and os.tosid = 1
										and t.scsid = ".$_SESSION["fabrica_var"]["scsid"]." order by t.odsid";
								$cabecalho = array("Ação","Tipo de Termo","Nº OS","Emitido por","Data Emissão");
								$db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'N', '100%', 'N' );
							?>
						</td>
					</tr>					
				 </table>
				<br>
				 <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" border="0">
					<tr>
						<td class="TituloTela" bgcolor="#DCDCDC">
							<b>Ordem de Serviços (OS Contagem de PF Estimado)</b>
						</td>
					</tr>
					<tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
						<td>
							<?php 
								$sql = "select 
											'<center><img src=\"/imagens/consultar.gif\" border=0 title=\"Visualizar\" style=\"cursor:pointer;\" onclick=\"window.open(\'fabrica.php?modulo=principal/providencias&acao=A&terid='||t.terid||'\',\'termo\', \'height=600, width=780, scrollbars=1\');\"></center>' as acao,
											tt.descricao as tipo,
											'<center>'||t.odsid||'</center>' as os,
											'<center>'||u.usunome||'</center>' as nome,
											'<center>'||to_char(t.data::timestamp,'DD/MM/YYYY HH24:MI:SS')||'</center>' as dataemissao
										from fabrica.termo t
										inner join fabrica.tipotermo tt on tt.tptid = t.tptid
										inner join fabrica.ordemservico os on os.odsid = t.odsid
										inner join seguranca.usuario u on u.usucpf = t.usucpf
										inner join (select max(terid) as terid, odsid, tptid from fabrica.termo group by odsid,tptid) t2 on t2.terid=t.terid and t2.tptid = t.tptid
										where  t.odsid is not null and os.tosid = 2
										and t.scsid = ".$_SESSION["fabrica_var"]["scsid"]." order by t.odsid";
								$cabecalho = array("Ação","Tipo de Termo","Nº OS","Emitido por","Data Emissão");
								$db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'N', '100%', 'N' );
							?>
						</td>
					</tr>					
				 </table>
				<br>
				 <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" border="0">
					<tr>
						<td class="TituloTela" bgcolor="#DCDCDC">
							<b>Ordem de Serviços (OS Contagem de PF Detalhada)</b>
						</td>
					</tr>
					<tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
						<td>
							<?php 
								$sql = "select 
											'<center><img src=\"/imagens/consultar.gif\" border=0 title=\"Visualizar\" style=\"cursor:pointer;\" onclick=\"window.open(\'fabrica.php?modulo=principal/providencias&acao=A&terid='||t.terid||'\',\'termo\', \'height=600, width=780, scrollbars=1\');\"></center>' as acao,
											tt.descricao as tipo,
											'<center>'||t.odsid||'</center>' as os,
											'<center>'||u.usunome||'</center>' as nome,
											'<center>'||to_char(t.data::timestamp,'DD/MM/YYYY HH24:MI:SS')||'</center>' as dataemissao
										from fabrica.termo t
										inner join fabrica.tipotermo tt on tt.tptid = t.tptid
										inner join fabrica.ordemservico os on os.odsid = t.odsid
										inner join seguranca.usuario u on u.usucpf = t.usucpf
										inner join (select max(terid) as terid, odsid, tptid from fabrica.termo group by odsid,tptid) t2 on t2.terid=t.terid and t2.tptid = t.tptid
										where  t.odsid is not null and os.tosid = 3
										and t.scsid = ".$_SESSION["fabrica_var"]["scsid"]." order by t.odsid";
								$cabecalho = array("Ação","Tipo de Termo","Nº OS","Emitido por","Data Emissão");
								$db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'N', '100%', 'N' );
							?>
						</td>
					</tr>					
				 </table>					 				 
			</td>
		</tr>
	</table>
</form>