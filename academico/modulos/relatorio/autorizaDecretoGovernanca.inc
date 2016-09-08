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
		$arWhere[] = " ent.entid " . (($unidade_campo_excludente == null || $unidade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $unidade ) . "') ";
	}
	
	if( $epcid[0] && ( $epcid_campo_flag || $epcid_campo_flag == '1' )){
		$arWhere[] = " epc.epcid " . (($epcid_campo_excludente == null || $epcid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $epcid ) . "') ";
	}
		
	if( $esdid[0] && ( $esdid_campo_flag || $esdid_campo_flag == '1' )){
		$arWhere[] = " esd.esdid " . (($esdid_campo_excludente == null || $esdid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esdid ) . "') ";
	}
	
	if( $mdlid[0] && ( $mdlid_campo_flag || $mdlid_campo_flag == '1' )){
		$arWhere[] = " mdl.mdlid " . (($mdlid_campo_excludente == null || $mdlid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $mdlid ) . "') ";
	}
	
	if( $tpcid ){
		$arWhere[] = " sbs.tpcid = ".$tpcid;
	}
		
	$sql = " SELECT
			       ent.entnome,
			       epc.epcdsc,
			       mdl.mdldsc,
			       sbs.sbsobjeto,
			       sbs.sbsvalor,
			       sbs.sbsjustificativa,			       
				   esd.esddsc 
			FROM academico.solicitacaobensservicos sbs 
				LEFT JOIN workflow.documento  			   doc on doc.docid = sbs.docid  
				LEFT JOIN workflow.historicodocumento	   hst on hst.hstid = doc.hstid
				LEFT JOIN workflow.estadodocumento 	   	   esd on esd.esdid = doc.esdid
				LEFT JOIN academico.modalidadelicitacao    mdl on sbs.mdlid = mdl.mdlid  
				LEFT JOIN academico.tipocontratomodalidade tcm on mdl.mdlid = tcm.mdlid 
				LEFT JOIN academico.tipocontrato           tpc on sbs.tpcid = tpc.tpcid
				LEFT JOIN academico.especiecontratacao     epc on sbs.epcid = epc.epcid 
				LEFT JOIN entidade.entidade     		   ent on ent.entid = sbs.entid
			WHERE sbs.sbsstatus = 'A' 
			".( is_array($arWhere) ? ' AND '.implode(' AND ', $arWhere) : '' )."
			ORDER BY sbs.sbsid";

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
							SIMEC- Sistema Integrado de Monitoramento Execução e Controle<br/>	MEC / SE - Secretaria Executiva <br />
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
				Autorização - Decreto de Governança				
			</td>
		</tr>
		<TR style="background:#D9D9D9;">		   
		   <TD align="center" valign="top" style="font-weight:bold;">Unidade</TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Especie da Contratação</TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Modalidade da Licitação</TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Objeto</TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Valor</TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Justificativa</TD>
		   <TD align="center" valign="top" style="font-weight:bold;">Situação</TD>
		</TR>
		';
		
		if($dados){
			
			$cor='#F7F7F7';
			
			foreach($dados as $d){
				
				if($cor=='#F7F7F7') $cor = '#FFFFFF';
				else $cor='#F7F7F7';
				
				$html .= '
				<tr bgcolor="'.$cor.'">
				   <td align="left" valign="top">'.$d['entnome'].'</td>
				   <td align="center" valign="top">'.$d['epcdsc'].'</td>
				   <td align="center" valign="top">'.$d['mdldsc'].'</td>
				   <td align="center" valign="top">'.$d['sbsobjeto'].'</td>
				   <td align="center" valign="top">'.formata_valor($d['sbsvalor']).'</td>
				   <td align="center" valign="top">'.$d['sbsjustificativa'].'</td>
				   <td align="left" valign="top">'.$d['esddsc'].'</td>
				</tr>
				';
			}		
		}
		else{
			$html .= '<tr><td align="center" colspan="7" style="font-weight:bold;">Não existem registros.</td></tr>';
		}
		
   $html .= '
   </table>
   
   <center>
   		<font size="2">   		
	   		<p>Brasília, '.date("d").' de '.retornaMes(date("m")).' de '.date("Y").'.</p>	   		
	   		<p>______________________________________________';
   
   if($tpcid == 1){
    $html .= "
        <br> ALOIZIO MERCADANTE OLIVA <br>
            Ministro de Estado da Educação
        </p>
    ";
   }else{
        $html .= "
            <br> LUIZ CLÁUDIO COSTA <br>
                Secretário Executivo
            </p>
        ";
   }
   
    $html .= '
        </font>   		
            <div class="noprint">
                <br/> 
                <input type="button" name="btn_imprimir2" id="btn_imprimir2" value="Imprimir" onclick="self.print();">
            </div>	
       </center>';

    echo $html;
   
//	include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
//   
//	ob_clean();
//	
//	$html = utf8_encode($html);		
//	$http = new RequestHttp();	
//	$http->toPdfDownload($html,"autorizacao_viagens_".date("d_m_Y"));	
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Autorização - Decreto de Governança', '&nbsp;' );

?>
<script type="text/javascript">

	function gerarRelatorio(req){
		
		var formulario = document.formulario;
		
		document.getElementById('req').value = req;
				
		selectAllOptions( formulario.unidade );
		selectAllOptions( formulario.esdid );
		selectAllOptions( formulario.epcid );
		selectAllOptions( formulario.mdlid );
				
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
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="req" id="req" value="" />	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="subtituloDireita">Tipo de Contrato</td>
			<td>
			<?php			
			
			$sql = "select 
						tpcid  as codigo,
						tpcdsc as descricao
					from 
						academico.tipocontrato
					where 
						tpcstatus = 'A'";
			
			$db->monta_combo('tpcid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'tpcid', false);	
			
			?>
			</td>
		</tr>
		<?php 
		
			// unidades
			$stSql = " select 
							entid as codigo,
							entnome as descricao 
						from 
							entidade.entidade 
						where 
							entunicod is not null 
						and 
							entstatus = 'A' 
						order by 
							entnome";
			
			$stSqlCarregados = "";
			mostrarComboPopup( 'Unidade', 'unidade',  $stSql, $stSqlCarregados, 'Selecione a(s) Unidade(s)' );
			
			$stSql = "select 
						epcid as codigo,
						epcdsc as descricao
					from 
						academico.especiecontratacao
					order by
						epcdsc";
			
			$stSqlCarregados = "";
			mostrarComboPopup( 'Espécie da Contratação', 'epcid',  $stSql, $stSqlCarregados, 'Selecione a(s) Espécie(s) da Contratação(ões)' );
			
			$stSql = "select 
						mdlid as codigo,
						mdldsc as descricao
					from 
						academico.modalidadelicitacao
					order by
						mdldsc";
			
			$stSqlCarregados = "";
			mostrarComboPopup( 'Modalidade da Licitação', 'mdlid',  $stSql, $stSqlCarregados, 'Selecione a(s) Modalidade(s) da Licitação' );
		
			$stSql = "select 
						esdid as codigo,
						esddsc as descricao
					from 
						workflow.estadodocumento
					where 
						tpdid = 71
					order by
						esddsc";
			
			$stSqlCarregados = "";
			mostrarComboPopup( 'Situação', 'esdid',  $stSql, $stSqlCarregados, 'Selecione a(s) Situação(ões)' );
			
			
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