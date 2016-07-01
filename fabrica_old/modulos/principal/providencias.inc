<?php

if($_REQUEST['terid']){
	echo $db->pegaUm("select corpo from fabrica.termo where terid = ".$_REQUEST['terid']);
	die;
}

include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

echo montarAbasArray(carregarMenuAnaliseSolicitacao(), $_SERVER['REQUEST_URI']);



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

                                             '<div style=\"white-space: nowrap; color: #0066CC;\">'
                                                || '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||t.scsid||'&ansid='||a.ansid|| '\'\">'||t.scsid||'</p>' 
                                            || '</div>' AS ss,

											'<center>'||u.usunome||'</center>' as nome,
											'<center>'||to_char(t.data::timestamp,'DD/MM/YYYY HH24:MI:SS')||'</center>' as dataemissao
										from fabrica.termo t
										inner join fabrica.tipotermo tt on tt.tptid = t.tptid
										inner join seguranca.usuario u on u.usucpf = t.usucpf
										inner join (select max(terid) as terid, scsid, tptid from fabrica.termo group by scsid,tptid) t2 on t2.terid=t.terid and t2.tptid = t.tptid

										  LEFT JOIN fabrica.ordemservico os ON os.scsid=t.scsid
										  LEFT JOIN fabrica.analisesolicitacao a ON a.scsid=os.scsid
										  LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid=a.scsid
										  LEFT JOIN  workflow.documento as wkd on wkd.docid = ss.docid

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

//permissao PERFIL_PREPOSTO, PERFIL_ESPECIALISTA_SQUADRA
$arrPerfil = pegaPerfilGeral();
$botoesContagem = 1;
if( (in_array(PERFIL_PREPOSTO,$arrPerfil) || in_array(PERFIL_ESPECIALISTA_SQUADRA,$arrPerfil)) && !in_array(PERFIL_SUPER_USUARIO,$arrPerfil) && !in_array(PERFIL_ADMINISTRADOR, $pfls)){
    $botoesContagem = 0;
}

								$sql = "select
											'<center><img src=\"/imagens/consultar.gif\" border=0 title=\"Visualizar\" style=\"cursor:pointer;\" onclick=\"window.open(\'fabrica.php?modulo=principal/providencias&acao=A&terid='||t.terid||'\',\'termo\', \'height=600, width=780, scrollbars=1\');\"></center>' as acao,
											tt.descricao as tipo,

                                                '<div style=\"white-space: nowrap; color: #0066CC;\">'
                                                    ||
                                                    (CASE WHEN t.odsid = ".TIPO_OS_GERAL." THEN '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.open(\'fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='||t.odsid||'\',\'Observacoes\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\">'||t.odsid||'</p>'
                                                          ELSE
                                                               (CASE WHEN wkd.esdid != ".WF_ESTADO_CPF_CANCELADA." AND 1=$botoesContagem THEN
                                                                        '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||t.odsid||'&scsid='||t.scsid || '\'\">'||t.odsid||'</p>'
                                                                     ELSE
                                                                        '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||t.odsid||'&scsid='||t.scsid || '\'\">'||t.odsid||'</p>'
                                                                END )
                                                     END)
                                                 || '</div>' AS os,

											'<center>'||u.usunome||'</center>' as nome,
											'<center>'||to_char(t.data::timestamp,'DD/MM/YYYY HH24:MI:SS')||'</center>' as dataemissao
										from fabrica.termo t
										inner join fabrica.tipotermo tt on tt.tptid = t.tptid
										inner join fabrica.ordemservico os on os.odsid = t.odsid
										inner join seguranca.usuario u on u.usucpf = t.usucpf
										inner join (select max(terid) as terid, odsid, tptid from fabrica.termo group by odsid,tptid) t2 on t2.terid=t.terid and t2.tptid = t.tptid

										  LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid=t.scsid
										  LEFT JOIN  workflow.documento as wkd on wkd.docid = ss.docid

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
                                                '<div style=\"white-space: nowrap; color: #0066CC;\">'
                                                    ||
                                                    (CASE WHEN t.odsid = ".TIPO_OS_GERAL." THEN '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.open(\'fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='||t.odsid||'\',\'Observacoes\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\">'||t.odsid||'</p>'
                                                          ELSE
                                                               (CASE WHEN wkd.esdid != ".WF_ESTADO_CPF_CANCELADA." AND 1=$botoesContagem THEN
                                                                        '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||t.odsid||'&scsid='||t.scsid || '\'\">'||t.odsid||'</p>'
                                                                     ELSE
                                                                        '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||t.odsid||'&scsid='||t.scsid || '\'\">'||t.odsid||'</p>'
                                                                END )
                                                     END)
                                                 || '</div>' AS os,
											'<center>'||u.usunome||'</center>' as nome,
											'<center>'||to_char(t.data::timestamp,'DD/MM/YYYY HH24:MI:SS')||'</center>' as dataemissao
										from fabrica.termo t
										inner join fabrica.tipotermo tt on tt.tptid = t.tptid
										inner join fabrica.ordemservico os on os.odsid = t.odsid
										inner join seguranca.usuario u on u.usucpf = t.usucpf
										inner join (select max(terid) as terid, odsid, tptid from fabrica.termo group by odsid,tptid) t2 on t2.terid=t.terid and t2.tptid = t.tptid

                                            LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid=t.scsid
                                            LEFT JOIN  workflow.documento as wkd on wkd.docid = ss.docid

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
                                                '<div style=\"white-space: nowrap; color: #0066CC;\">'
                                                    ||
                                                    (CASE WHEN t.odsid = ".TIPO_OS_GERAL." THEN '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.open(\'fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='||t.odsid||'\',\'Observacoes\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\">'||t.odsid||'</p>'
                                                          ELSE
                                                               (CASE WHEN wkd.esdid != ".WF_ESTADO_CPF_CANCELADA." AND 1=$botoesContagem THEN
                                                                        '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||t.odsid||'&scsid='||t.scsid || '\'\">'||t.odsid||'</p>'
                                                                     ELSE
                                                                        '<p title=\"Editar O.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||t.odsid||'&scsid='||t.scsid || '\'\">'||t.odsid||'</p>'
                                                                END )
                                                     END)
                                                 || '</div>' AS os,
											'<center>'||u.usunome||'</center>' as nome,
											'<center>'||to_char(t.data::timestamp,'DD/MM/YYYY HH24:MI:SS')||'</center>' as dataemissao
										from fabrica.termo t
										inner join fabrica.tipotermo tt on tt.tptid = t.tptid
										inner join fabrica.ordemservico os on os.odsid = t.odsid
										inner join seguranca.usuario u on u.usucpf = t.usucpf
										inner join (select max(terid) as terid, odsid, tptid from fabrica.termo group by odsid,tptid) t2 on t2.terid=t.terid and t2.tptid = t.tptid

                                            LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid=t.scsid
                                            LEFT JOIN  workflow.documento as wkd on wkd.docid = ss.docid

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