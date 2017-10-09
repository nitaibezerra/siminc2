<?php

function retornaMes($num)
{
	switch($num) {
		case "01": $mes = "Janeiro";   break;
		case "02": $mes = "Fevereiro"; break;
		case "03": $mes = "Março";     break;
		case "04": $mes = "Abril";     break;
		case "05": $mes = "Maio";      break;
		case "06": $mes = "Junho";     break;
		case "07": $mes = "Julho";     break;
		case "08": $mes = "Agosto";    break;
		case "09": $mes = "Setembro";  break;
		case "10": $mes = "Outubro";   break;
		case "11": $mes = "Novembro";  break;
		case "12": $mes = "Dezembro";  break;
	}
	return $mes;
}

if ($_POST){
	
	
	extract($_POST);
	
	if( $unidade[0] && ( $unidade_campo_flag || $unidade_campo_flag == '1' )){
		$whereUnidade[] = " a.entid " . (($unidade_campo_excludente == null || $unidade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $unidade ) . "') ";
	}
	
	if( $ano[0] && ( $ano_campo_flag || $ano_campo_flag == '1' )){
		$where[] = " to_char(a.avedata,'YYYY') " . (($ano_campo_excludente == null || $ano_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ano ) . "') ";
	}
		
	if( $situacao[0] && ( $situacao_campo_flag || $situacao_campo_flag == '1' )){
		$where[] = " d.esdid " . (($situacao_campo_excludente == null || $situacao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $situacao ) . "') ";
	}
		
	
	$sql = " select
					ent.entid,
					ent.entunicod ||' - '|| ent.entnome as nomeunidade
				from 
					academico.autviagemexterior a
				inner join 
					entidade.entidade ent ON ent.entid = a.entid
				inner join 
					workflow.documento d ON d.docid = a.docid
				where 
					avestatus = 'A'
					" . (is_array($whereUnidade) ? " AND ".implode(' AND ', $whereUnidade) : '') . "
					" . (is_array($where) ? " AND ".implode(' AND ', $where) : '') . "
				group by 
					ent.entid, ent.entunicod, ent.entnome
				order by
					ent.entnome ";
	
	/*
	$sql = "select
				entnome as unidade,
				tbl1.aveid as nusolicitacao,
				esddsc as situacao,
				ed.esdid,
				ent.entid,
				avenomereitor as reitor,
				avenumauto as nuautorizacao,
				pvedsc as programa,
				--( select sum(qtde) from academico.autvexqtdeprg atp where atp.aveid = tbl1.aveid) as qtde_ci,
				qtde as qtde_solicitada,
				to_char(avedata,'DD/MM/YYYY') as datasolicitacao,
				'1' as qtde_reg,
				'Detalhes da Solicitação Nº '||tbl1.aveid as detalhessolicitacao
			from 
				academico.autviagemexterior tbl1
			inner join 
				academico.autvexqtdeprg au on au.aveid = tbl1.aveid
			inner join 
				academico.progviagemexterior pg on pg.pveid = au.pveid
			inner join 
				entidade.entidade ent ON tbl1.entid = ent.entid
			inner join 
				workflow.documento d ON d.docid = tbl1.docid
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where 
				avestatus = 'A'
				" . (is_array($where) ? " AND ".implode(' AND ', $where) : '') . "
			ORDER BY 
				" . (is_array($agrupador) ? implode(' , ', $agrupador)."," : '') . "
				tbl1.aveid ";	
	*/
	$dados = $db->carregar($sql);
	
	
	$html = '
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	<style type="text/css">
    	@media print {
      		.noprint { display: none; }
    	}
  	</style>
	
	<div class="noprint" align="center">
		<table width="95%">
			<tr>
				<td align="right">
	   				<input type="button" name="btn_imprimir" id="btn_imprimir" value="Imprimir" onclick="self.print();">
	   			</td>
	   		</tr>
	   	</table>
   	</div>
   	
	<table width="100%" cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">
		<tr>
			<td colspan="100">
				<center>
				<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
					<tr bgcolor="#ffffff">
						<td valign="top" width="50" rowspan="2">
							<img src="../imagens/brasao.gif" width="45" height="45" border="0">
						</td>
						<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
							'. NOME_SISTEMA. '<br/>	MEC / SE - Secretaria Executiva <br />
						</td>
						<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
							Impresso por: <b>'.$_SESSION['usunome'].'</b><br/>
							Date e Hora da Impressão: '.date("d/m/Y H:i:s").'<br />
						</td>
					</tr>
					<tr>
						<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
							<b><font style="font-size:14px;"></font></b>
						</td>
					</tr>
				</table>
				</center>
			</td>
		</tr>
		<tr>
			<td colspan="7" align="center" style="font-weight:bold;">
				Autorização para Diárias e Passagens para Viagens para Exterior
				';
					if( $ano[0] ){
							$html .= " - ". implode( "/", $ano );
					}
		$html .= '
			</td>
		</tr>
		<TR style="background:#D9D9D9;">
		   <TD>
				 <div style="font-weight:bold;"><div >Unidade</div></div>
		   </TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Cooperação Internacional</TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Intercâmbio Acadêmico</TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Pós-Graduação e Inovação</TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Treinamento, Capacitação e Qualificação</TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Total</TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Situação</TD>
		</TR>
		';
		
		if($dados){
			
			$cor='#F7F7F7';
			
			foreach($dados as $d){
				
				if($cor=='#F7F7F7') $cor = '#FFFFFF';
				else $cor='#F7F7F7';
				
				$subSql = "select a.aveid 
							 from academico.autviagemexterior a
							 inner join 
								workflow.documento d ON d.docid = a.docid
							 inner join 
								workflow.estadodocumento ed on ed.esdid = d.esdid
							 where a.avestatus = 'A' 
							 and a.entid = ".$d['entid'] . (is_array($where) ? " AND ".implode(' AND ', $where) : '');
				

				//1;"Treinamento, Capacitação e Qualificação"
				$sql = "select sum(qtde) from academico.autvexqtdeprg where pveid = 1 and aveid in ({$subSql})";
				$qtd_tcq = $db->pegaUm($sql);
				if(!$qtd_tcq) $qtd_tcq = 0;
				
				//2;"Intercâmbio Acadêmico"
				$sql = "select sum(qtde) from academico.autvexqtdeprg where pveid = 2 and aveid in ({$subSql})";
				$qtd_ia = $db->pegaUm($sql);
				if(!$qtd_ia) $qtd_ia = 0;
				
				//3;"Cooperação Internacional"
				$sql = "select sum(qtde) from academico.autvexqtdeprg where pveid = 3 and aveid in ({$subSql})";
				$qtd_ci = $db->pegaUm($sql);
				if(!$qtd_ci) $qtd_ci = 0;
				
				//4;"Pós-Graduação e Inovação"
				$sql = "select sum(qtde) from academico.autvexqtdeprg where pveid = 4 and aveid in ({$subSql})";
				$qtd_pi = $db->pegaUm($sql);
				if(!$qtd_pi) $qtd_pi = 0;
				
				$total_linha = $qtd_tcq + $qtd_ia + $qtd_ci + $qtd_pi;
				$total_geral += $total_linha;
				
				$total_coluna_tcq += $qtd_tcq;
				$total_coluna_ia += $qtd_ia;
				$total_coluna_ci += $qtd_ci;
				$total_coluna_pi += $qtd_pi;
				
				$html .= '
				<TR BGCOLOR="'.$cor.'">
				   <TD align="left" valign="top">'.$d['nomeunidade'].'</TD>
				   <TD align="center" valign="top">'.$qtd_ci.'</TD>
				   <TD align="center" valign="top">'.$qtd_ia.'</TD>
				   <TD align="center" valign="top">'.$qtd_pi.'</TD>
				   <TD align="center" valign="top">'.$qtd_tcq.'</TD>
				   <TD align="center" valign="top">'.$total_linha.'</TD>
				   <TD align="left" valign="top">
				   ';
							$sql = "select
										ed.esdid, ed.esddsc
									from 
										academico.autviagemexterior a
									inner join 
										workflow.documento d ON d.docid = a.docid
									inner join 
										workflow.estadodocumento ed on ed.esdid = d.esdid
									where 
										avestatus = 'A'
										and a.entid = ".$d['entid'] . ($where ? " and ".implode(" and ",$where) : "")."
									group by 
										ed.esdid, ed.esddsc
									order by
										ed.esddsc";
							$situacao = $db->carregar($sql);
							
							if($situacao){
								
								foreach($situacao as $s){
									
									$sql = "select
												sum(qtde) as totalqtd
											from 
												academico.autviagemexterior a
											inner join 
												workflow.documento d ON d.docid = a.docid
											inner join 
												academico.autvexqtdeprg atp on atp.aveid = a.aveid
											where 
												avestatus = 'A'
												and a.entid = ".$d['entid']."
												and d.esdid = ".$s['esdid'] . ($where ? " and ".implode(" and ",$where) : "")."
											";
									$qtd = $db->pegaUm($sql);
									
									$html .= $s['esddsc'] . ": ". $qtd . "<br>";
									
								}
							} 
							
				   		$html .= '
				   </TD>
				</TR>
				';
			}
			
			$html .= '
			
			<TR style="background:#D9D9D9;">
			   <TD align="right" valign="top" style="font-weight:bold;">TOTAL:</TD>
			   <TD align="center" valign="top" style="font-weight:bold;">'.$total_coluna_ci.'</TD>
			   <TD align="center" valign="top" style="font-weight:bold;">'.$total_coluna_ia.'</TD>
			   <TD align="center" valign="top" style="font-weight:bold;">'.$total_coluna_pi.'</TD>
			   <TD align="center" valign="top" style="font-weight:bold;">'.$total_coluna_tcq.'</TD>
			   <TD align="center" valign="top" style="font-weight:bold;">'.$total_geral.'</TD>
			   <TD align="center" valign="top" style="font-weight:bold;">&nbsp;</TD>
			</TR>
		
			';
		}
		else{
			$html .= '<TR><TD align="center" colspan="7" style="font-weight:bold;">Não existem registros.</TD></TR>';
		}
		
		
   $html .= '
   </table>
   
   <center>
   		<table width="80%">
   			<tr>
   				<td align="center">
   					Brasília, '.date("d").' de '.retornaMes(date("m")).' de '.date("Y").'.
   				</td>
   				<td align="center">
   					<br><br>______________________________________________
   					<br>JOSÉ HENRIQUE PAIM FERNANDES
					<br>Secretário Executivo
   				</td>
   			</tr>
   		</table>
   
   		<div class="noprint">
	   		<br><br> <input type="button" name="btn_imprimir2" id="btn_imprimir2" value="Imprimir" onclick="self.print();">
	   		<!-- 
	   		&nbsp;&nbsp; <input type="button" name="btn_salvar" id="btn_salvar" value="Salvar" onClick="window.location.href=window.location.href">
	   		-->
   		</div>	
		
   
   </center>
   ';
   
   echo $html;
   
   include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
   
    //$html = ob_get_contents();
	ob_clean();
	//echo $html;
	
	$html = utf8_encode($html);
		
	$http = new RequestHttp();
	//$http->toPdf( $html );
	$http->toPdfDownload($html,"autorizacao_viagens_".date("d_m_Y"));
		
		
		
		
	
	
	
	/*
	$content = http_build_query( array('conteudoHtml'=> utf8_encode($html) ) );
	$context = stream_context_create(array( 'http'    => array(
							                'method'  => 'POST',
							                'content' => $content ) ));

	$contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );
	header('Content-Type: application/pdf');
	header("Content-Disposition: attachment; filename='autorizacao_geral_".date("dmY")."'");
	echo $contents;
	*/
	
   exit;
}

	
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Autorização de Viagens para o Exterior', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
function gerarRelatorio(req){
	var formulario = document.formulario;

	document.getElementById('req').value = req;
	
	selectAllOptions( formulario.ano );
	selectAllOptions( formulario.unidade );
	selectAllOptions( formulario.situacao );
			
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	

	formulario.submit();
	
	janela.focus();
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name="req" id="req" value="" />	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<?php
			
				// ano
				$anoini = 2011;
				$anofim = date('Y');
				$arrAno = array();
				$stSql = "SELECT $anoini as codigo, $anoini as descricao";
				for($anoini+1; $anoini<=$anofim; $anoini++){
					$stSql .= " UNION
								SELECT $anoini as codigo, $anoini as descricao
							  ";
				}
				$stSql .= " ORDER BY 1 ";
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Ano', 'ano',  $stSql, $stSqlCarregados, 'Selecione o(s) Ano(s)' ); 

				
				// unidades
				$stSql = " select
								ent.entid as codigo,
								ent.entnome as descricao
							from 
								academico.autviagemexterior tbl1
							inner join 
								entidade.entidade ent ON tbl1.entid = ent.entid
							where 
								avestatus = 'A'
							group by 
								ent.entid, ent.entnome
							order by
								entnome ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Unidade', 'unidade',  $stSql, $stSqlCarregados, 'Selecione a(s) Unidade(s)' ); 
				
				// Situação
				/*
				$sql = "SELECT esdid as codigo, esddsc as descricao
						FROM workflow.estadodocumento e
						INNER JOIN workflow.tipodocumento t on t.tpdid = e.tpdid
						WHERE esdstatus = 'A' and sisid = ".$_SESSION['sisid']."
						ORDER BY esddsc";
				*/
				$sql = "select	
							ed.esdid as codigo,
							ed.esddsc as descricao
						from 
							academico.autviagemexterior tbl1
						inner join 
							workflow.documento d ON d.docid = tbl1.docid
						inner join 
							workflow.estadodocumento ed on ed.esdid = d.esdid
						where 
							avestatus = 'A'
						group by 
							ed.esdid, ed.esddsc
						order by
							ed.esddsc";
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Situação', 'situacao',  $sql, $stSqlCarregados, 'Selecione a(s) Situação(ões)' ); 
				
			?>
	<tr>
		<td width="40%" align="left">
			&nbsp;
		</td>
		<td align="left">
			<input type="button" name="Gerar" value="Gerar" onclick="javascript:gerarRelatorio(1);"/>
		</td>
	</tr>
</table>
</form>
</body>
</html>