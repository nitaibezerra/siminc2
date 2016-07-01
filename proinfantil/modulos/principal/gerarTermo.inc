<?

if($_REQUEST['requisicao'] == '3'){
	$sql = "delete FROM proinfantil.termoadesao	WHERE pinid = $pinid";
	$db->executar($sql);
	
	
    
    $sql = "INSERT INTO proinfantil.termoadesao(pinid, terid, teadatainclusao, usucpf, arqid, teaadesao)
    		VALUES ($pinid,
    		 		1,
    		 		'".date('Y-m-d')."',
    		 		'".$_SESSION['usucpf']."',
    		 		null,
    		 		true)"; 
	$db->executar($sql);					
	
	$db->commit();
}


function mes_extenso2($mes)
{
	if (strval($mes) == 1) return 'janeiro';
	else   if (strval($mes) == 2) return 'fevereiro';
	else   if (strval($mes) == 3) return 'março';
	else   if (strval($mes) == 4) return 'abril';
	else   if (strval($mes) == 5) return 'maio';
	else   if (strval($mes) == 6) return 'junho';
	else   if (strval($mes) == 7) return 'julho';
	else   if (strval($mes) == 8) return 'agosto';
	else   if (strval($mes) == 9) return 'detembro';
	else   if (strval($mes) == 10) return 'outubro';
	else   if (strval($mes) == 11) return 'novembro';
	else   if (strval($mes) == 12) return 'dezembro';
}


$sql = "select
			upper(ee.entnome) as entnome,
	        upper(oi.obrnome) as obrdesc,
			tm.mundescricao,
			tm.estuf,
			COALESCE(tpl.tpodsc,'N/A') as tpodsc,
			ee.entnumcpfcnpj as cnpj,
			ee.entcodent as codinep
		FROM
			obras2.obras AS oi
		INNER JOIN
		    entidade.entidade AS ee ON oi.entid = ee.entid
		INNER JOIN
		    entidade.endereco AS ed ON oi.endid = ed.endid
		INNER JOIN
		    territorios.municipio AS tm ON ed.muncod = tm.muncod
		LEFT JOIN obras2.tipologiaobra AS tpl ON oi.tpoid = tpl.tpoid AND tpl.tpostatus = 'A'
		where
			oi.obrid = $obrid
		".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");
$arrDados = $db->pegaLinha($sql);
if($arrDados){
	extract($arrDados);
}


$sql = "select
				pinperiodorepasse, pinpareceraprovacao
			FROM
				proinfantil.proinfantil
			WHERE
				pinid = $pinid";
$dados = $db->pegaLinha($sql);
if($dados) extract($dados);	


$cnpj = $cnpj ? formatar_cnpj($cnpj) : "";


$html = '<style type="text/css">
			.Tabela
			{
			    FONT-SIZE: xx-small;
			    FONT-FAMILY: Arial, Verdana;
			    FONT-SIZE: 8pt;
			    BORDER-RIGHT: #cccccc 1px solid;
			    BORDER-TOP: #cccccc 1px solid;
			    BORDER-LEFT: #cccccc 1px solid;
			    BORDER-BOTTOM: #cccccc 1px solid;
				TEXT-DECORATION: none;
				WIDTH: 95%;
				TEXT-COLOR: #000000;
			}
			
			.SubTituloDireita
			{
			    FONT-SIZE: 8pt;
			    COLOR: black;
			    FONT-FAMILY: Arial, Verdana;
			    TEXT-ALIGN: right;
				BACKGROUND-COLOR: #dcdcdc;
			}
			.SubTituloEsquerda
			{
			    FONT-WEIGHT: bold;
			    FONT-SIZE: 8pt;
			    COLOR: black;
			    FONT-FAMILY: Arial, Verdana;
			    BACKGROUND-COLOR: #dcdcdc;
			    TEXT-ALIGN: left
			}
			
			.SubTituloCentro
			{
			    FONT-WEIGHT: bold;
			    FONT-SIZE: 8pt;
			    COLOR: black;
			    FONT-FAMILY: Arial, Verdana;
			    BACKGROUND-COLOR: #f0f0f0;
			    TEXT-ALIGN: center
			}
		</style>';

$html .= '<center>
			<img src="../imagens/brasao.gif" width="85" height="85" border="0">
			<BR>
			<font style="font-size: 14px;font-family: arial, sans-serif">
				<B>
					MINISTÉRIO DA EDUCAÇÃO
					<BR>
					SECRETÁRIA DE EDUCAÇÃO BÁSICA - SEB
					<br>
					DIRETORIA DE CURRÍCULOS E EDUCAÇÃO INTEGRAL - DICEI
					<BR>
					COORDENAÇÃO GERAL DE EDUCAÇÃO INFANTIL
				</B>
				<BR>
				Esplanada dos Ministérios, Bloco L, Edifício Sede, 5º Andar
				<br>
				CEP: 70.047-900 - Brasília, DF
		
				<BR><BR><BR><BR>
				
				<div align="left">
				<b>Análise Técnica - COEDI / DICEI / SEB / MEC</b>
				</div>
				
				<BR><BR>
			</font>
			
			<div align="right" style="font-size: 12px;font-family: arial, sans-serif">
				Brasilia, '.date("d").' de '.mes_extenso2(date("m")).' de '.date("Y").'
			</div>
			
			<br><br>
			
			<font style="font-size: 14px;font-family: arial, sans-serif">
				<b>Identificação</b>
			</font>
			
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
			        <td width="25%" class="SubTituloDireita">CNPJ:</td>
			        <td>
			        	' . $cnpj . '
			        </td>
			    </tr>
				<tr>
			        <td width="25%" class="SubTituloDireita">Nome da Instituição:</td>
			        <td>
			        	' . $entnome . '
			        </td>
			    </tr>
				<tr>
			        <td class="SubTituloDireita">Nome da Obra:</td>
			        <td>
			        	' . $obrdesc . '
			        </td>
			    </tr>
			    <tr>
			        <td class="SubTituloDireita">Município / UF:</td>
			        <td>
			        	' . $mundescricao . ' / ' . $estuf . '
			        </td>
			    </tr>
			    <tr>
			        <td class="SubTituloDireita">Tipologia:</td>
			        <td>
			        	' . $tpodsc . '
			        </td>
			    </tr>';


if($codigoinep){
	$html .= '	    <tr>
				        <td class="SubTituloDireita">Código INEP:</td>
				        <td>
				        	' . $codigoinep . '
				        </td>
				    </tr>';
}
			    
$html .= '			    <tr>
			        <td class="SubTituloDireita">Objeto da Análise:</td>
			        <td>
			        	xxx
			        </td>
			    </tr>
		
			</table>
			
			<br>';


$arrModalidades = recuperaTipoModalidade();
$arrAlunos = $pinid ? recuperaAlunoAtendido($pinid) : array();
$periodoTotalMeses = $pinperiodorepasse;
	
	
if($periodoTotalMeses){	

	$html .= '<table width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr >
				    	<td style="background-color: #C0C0C0;" colspan="5" class="SubTituloCentro">VALOR DO REPASSE</td>
				    </tr>				    
				    <tr >
				    	<td class="SubTituloEsquerda">Etapa</td>
				    	<td class="SubTituloEsquerda">Matrículas não Computadas <br>para Recebimento do Fundeb</td>
				    	<td class="SubTituloEsquerda">Valor Unitário</td>
				    	<td class="SubTituloEsquerda">Período Considerado para o Repasse<br> de Recursos (meses)</td>
				    	<td class="SubTituloEsquerda">Valor Total</td>
				    </tr>';
				    if($arrModalidades): 

				    	$somaColuna=0;
				    	$somaColuna4=0;
				    	$cor = "#FFFFFF";
				    	
				    	foreach($arrModalidades as $modalidade):

							$calculaLinha=0;
						    $cor = "#FFFFFF" ? "#f7f7f7" : "#FFFFFF";
						    $turno = 1;
						    $vlPeriodo = $db->pegaUm("select vaavalor as valor from proinfantil.valoraluno where vaastatus = 'A' and timid = ".$modalidade['timid']);
						    
						    $somaColuna += $arrAlunos[$turno][$modalidade['timid']];
							$calculaLinha = $arrAlunos[$turno][$modalidade['timid']];
							
							$valorTotal = ($arrAlunos[$turno][$modalidade['timid']] * ($vlPeriodo ? $vlPeriodo : 0) * $periodoTotalMeses);
							$somaColuna4 += $valorTotal;
								        								
							$vlPeriodo = $vlPeriodo ? number_format($vlPeriodo,2,",",".") : "";
							$valorTotal = $valorTotal ? number_format($valorTotal,2,",",".") : "0,00";
							
							$html .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor='.$cor.';" onmouseover="this.bgColor=#ffffcc;">
								        <td>'. $modalidade['timdescricao'] .' Integral</td>
								        <td>'. $arrAlunos[$turno][$modalidade['timid']] .'</td>
								        <td>'. $vlPeriodo .'</td>
								        <td>'. $periodoTotalMeses. '</td>
								        <td class="center bold" >'. $valorTotal .'</td>
								    </tr>';
								    
							$calculaLinha=0;
							$cor = "#f7f7f7" ? "#FFFFFF" : "#f7f7f7";
							$turno++;
							
							$somaColuna += $arrAlunos[$turno][$modalidade['timid']];
							$valorTotal = ($arrAlunos[$turno][$modalidade['timid']] * ($vlPeriodo ? $vlPeriodo : 0) * $periodoTotalMeses);
							$somaColuna4 += $valorTotal;
							
							$vlPeriodo = $vlPeriodo ? number_format($vlPeriodo,2,",",".") : "";
							$valorTotal = $valorTotal ? number_format($valorTotal,2,",",".") : "0,00";
						    
						    $html .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor='.$cor.';" onmouseover="this.bgColor=#ffffcc;">
								        <td>'. $modalidade['timdescricao'] .' Parcial</td>
								        <td>'. $arrAlunos[$turno][$modalidade['timid']] .'</td>
								        <td>'. $vlPeriodo .'</td>
								        <td>'. $periodoTotalMeses .'</td>
								        <td>'. $valorTotal .'</td>
								    </tr>';
						    
					    endforeach;
					    
					    $somaColuna4 = $somaColuna4 ? number_format($somaColuna4,2,",",".") : "0,00";
					    
					    $html .= '<tr >
							        <td class="SubTituloEsquerda" >Total Geral</td>
					        		<td class="SubTituloEsquerda" >'. $somaColuna .'</td>
							        <td class="SubTituloEsquerda" >&nbsp;</td>
							        <td class="SubTituloEsquerda" >&nbsp;</td>
							        <td class="SubTituloEsquerda" >'. $somaColuna4 .'</td>
							    </tr>';
				    endif;			
				    	    
				$html .= '</table>';
				
				$html .= '<BR>';
				
				$html .= '<table border="0" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
				    	<td align="left" style="font-size: 11px">
				    		(RESOLUÇÃO CD/FNDE Nº 52 DE 29 DE SETEMBRO DE 2011 - Art. 9º No ano de 2011, excepcionalmente,
				    		os estabelecimentos que iniciaram seu atendimento antes da publicação desta Resolução farão jus
				    		a, no máximo, 7/12 do valor aluno-ano definido pelo Fundeb para creche e pré-escola em período
				    		integral e parcial no exercício de 2010, conforme Portaria MEC 647, de 23 de maio de 2011.)
				    	</td>
				    </tr>
				</table>
				
				<BR>';
				
				
				$html .= '<table class="Tabela" border="0" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
				    	<td style="background-color: #C0C0C0;" class="SubTituloCentro">
				    		PARECER DE APROVAÇÃO
				    	</td>
				    </tr>
					<tr>
				    	<td>
				    		'.str_replace(chr(13),"<br>",$pinpareceraprovacao) .'
				    	</td>
				    </tr>				    
				</table>
				
				<BR>';
				
				$html .= '<table style="FONT-FAMILY: Arial, Verdana; FONT-SIZE: 8pt;" width="100%" cellSpacing="1" cellPadding="3" align="center">
					<tr>
				    	<td align="right" >
				    		<b>Usuário:</b> '.$_SESSION['usunome'].' 
				    		&nbsp;&nbsp;&nbsp;&nbsp;
				    		<b>Data:</b> '.date("d/m/Y H:i:s").'
				    	</td>
				    </tr>
				</table>';				
				
	
	} //fecha $periodoTotalMeses	
	
$html .= '<center>';



//insere arquivo pdf

echo $html;
