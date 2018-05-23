<?php
class reformulacaoPTA{
	private $db;
	
	public function __construct( $db = null ){
		if( empty($db) ){
			$this->db = new cls_banco();
		} else {
			$this->db = $db;
		}
	}
	
	public function carregaDadosIniciativa( $ptrid ){
		$acao = "<a style=\"cursor:pointer;\" onclick=\"manterDadosEspecificacao(\''||pti.ptiid||'\',\''||pti.ptrid||'\',\''||ini.iniid||'\');\">
					<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
				</a>";
		$sql = "SELECT
				  '".$acao."' as acao, 
				  pti.ptiid as codigo,
				  ini.ininome as descricao,
				  pti.ptivalortotal,
				  pti.ptivalorconcedente,
				  pti.ptivalorproponente
				FROM
				  emenda.v_ptiniciativa pti INNER JOIN emenda.iniciativa ini
				  ON (pti.iniid = ini.iniid)
				WHERE
				  pti.ptrid = ".$ptrid."
				  --and ini.inistatus = 'A'";
	
		return $this->db->carregar($sql);
	}
		
	public function pegaDataEspecificacao( $ptiid ){
		$sql = "SELECT 
				  to_char(min(ptedatainicio), 'DD/MM/YYYY') as min_dataini, 
  				  to_char(max(ptedatafim), 'DD/MM/YYYY') as max_datafim 
				FROM 
					emenda.ptiniciativaespecificacao
				WHERE 
					ptiid = $ptiid 
					and ptestatus = 'A'";
		
		return $this->db->pegaLinha( $sql );
	}
	
	public function cabecalhoEspecificacaoIniciativa( $boAjax = false ){
		//if( $_SESSION['emenda']['federal'] == 'N' ){
			$compl = "<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"8%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Valor Proponente</strong></td>";
		//}
		$html = "<thead>
			<tr>
				<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"25%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Especificação da Iniciativa</strong></td>
				<td colspan=\"2\" align=\"Center\" class=\"title\" width=\"10%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Indicador Físico</strong></td>
				<td colspan=\"2\" align=\"Center\" class=\"title\" width=\"18%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Custo</strong></td>
				$compl
				<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"8%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Valor Concedente</strong></td>
				<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"10%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Data Inicial</strong></td>
				<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"10%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Data Final</strong></td>
				".( $boAjax ? '' : "<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"15%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Ação</strong></td>")."
			</tr>
			<tr>
				<td align=\"Center\" class=\"title\" width=\"8%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Unidade de Medida</strong></td>
				<td align=\"Center\" class=\"title\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Quantidade</strong></td>
				<td align=\"Center\" class=\"title\" width=\"10%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Valor Unitário</strong></td>
				<td align=\"Center\" class=\"title\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Valor Total</strong></td>
			</tr>
		</thead>";
		
		return $html;
	}
	public function carregaIniciativaEmendaAjax($pteid, $ptrid){
		$sql = "SELECT DISTINCT 
				    vede.edeid, 
					vede.emecod,
					(CASE WHEN vede.emetipo = 'E' THEN 'Emenda'
						ELSE 'Complemento' END) as tipoemenda, 
				    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome, 
				    fup.fupdsc, 
				    pr.pervalor, 
				    ped.pedid
				FROM emenda.planotrabalho ptr
					INNER JOIN emenda.ptemendadetalheentidade  ped
				    	ON ptr.ptrid = ped.ptrid
				    INNER JOIN emenda.v_emendadetalheentidade vede
				    	ON vede.edeid = ped.edeid
				    LEFT JOIN emenda.v_funcionalprogramatica fup
				    	ON (vede.acaid = fup.acaid)
				    INNER JOIN emenda.autor aut
				    	ON aut.autid = vede.autid
				    INNER JOIN emenda.ptiniciativa pti
				    	on pti.ptrid = ptr.ptrid
				    inner join emenda.iniciativa ini
	                	on ini.iniid = pti.iniid
				    INNER JOIN emenda.ptiniesprecurso pr
						ON pr.pedid = ped.pedid 
						AND pr.pteid = $pteid
				WHERE ptr.ptrid = ".$ptrid."
				    AND vede.edestatus = 'A'";
	
		$arDados = $this->db->carregar($sql);
		
		$html = '<table id="tblform" class="listagem" width="70%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left">
			<thead>
			<tr>
				<td>&nbsp;</td>
				<td align="Center" class="title" width="20%"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Código</strong></td>
				<td align="Center" class="title" width="10"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Tipo</strong></td>
				<td align="Center" class="title" width="40%"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Autor</strong></td>
				<td align="Center" class="title" width="30%"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor</strong></td>
			</tr>
			</thead>';	
		
		if($arDados){
			foreach ($arDados as $key => $value) {
				$key % 2 ? $cor = "#dedfde" : $cor = "";
				$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
							<td><img src="/imagens/seta_filho.gif" border="0"></td>
							<td style="text-align: center;">'.$value['emecod'].'</td>
							<td style="text-align: center;">'.$value['tipoemenda'].'</td>
						  	<td style="text-align: center;">'.$value['autnome'].'</td>
						  	<td style="text-align: center;">R$ '.number_format($value['pervalor'], 2, ',', '.').'</td>
						</tr>';
			}
			
		} else{
			$html.= '<tr><td align="center" style="color:#cc0000;" colspan="3">Não foram encontrados Registros.</td></tr>';
		}	
		$html.= "</table>";

		return $html;
	}
	public function ListarbeneficiariosAcaoPlanoTrabalho($ptiid){
		
		$sql = "SELECT * FROM(
					SELECT DISTINCT
						ptib.ptbquantidaderural as rural,
					  	ptib.ptbquantidadeurbana as urbana,
					  	ben.bennome as beneficiario,
					  	(ptib.ptbquantidaderural + ptib.ptbquantidadeurbana) as total
					FROM
					  	emenda.ptiniciativa pti 
					  	INNER JOIN emenda.ptiniciativabeneficiario ptib 
					  		ON (pti.ptiid = ptib.ptiid) 
					    RIGHT JOIN emenda.iniciativabeneficiario inb
					  		ON (ptib.icbid = inb.icbid) 
					    INNER JOIN emenda.beneficiario ben
					  		ON (inb.benid = ben.benid)
					WHERE
					  	inb.icbstatus = 'A'
					  	AND ben.benstatus = 'A'
					   	".($ptiid ? " AND pti.ptiid = $ptiid " : "")." 
					
					UNION
					
					SELECT DISTINCT
						ptb.ptbquantidaderural as rural,
					  	ptb.ptbquantidadeurbana as urbana,
					  	b.bennome as beneficiario,
					  	(ptb.ptbquantidaderural + ptb.ptbquantidadeurbana) as total
					FROM
					  	emenda.ptiniciativa pt 
					  	INNER JOIN emenda.ptiniciativabeneficiario ptb 
					  		ON (pt.ptiid = ptb.ptiid) 
					    inner JOIN emenda.iniciativabeneficiario ib
					  		ON (ptb.icbid = ib.icbid) 
					    INNER JOIN emenda.beneficiario b
					  		ON (ib.benid = b.benid)
					WHERE
						".($ptiid ? " pt.ptiid = $ptiid " : "")."
					) as t
					ORDER BY
						beneficiario";
	
		$dadosben = $this->db->carregar($sql);
		
		$html = '';
		if($dadosben){
			$html.= "<table class='listagem' width='100%'>
						<thead>
							<tr>
								<td width='60%'></td>
								<td align='center'><b>Rural</b></td>
								<td align='center'><b>Urbana</b></td>
								<td align='center'><b>Total</b></td>
							</tr>
						</thead>
						<tbody>";
			foreach( $dadosben as $benef ){
				$html.= "<tr>
							<td align='center'><b>".$benef['beneficiario']."</b></td>
							<td align='center'>".$benef['rural']."</td>
							<td align='center'>".$benef['urbana']."</td>
							<td align='center'>".$benef['total']."</td>
						<tr>";
			}
			
			$html.= "</tbody></table>";
		} else {
			$html.="<table class='listagem' width='100%'>
						<tr style='text-align: center;'>
							<td>-</td>
						</tr>
					</table>";
		}
		
		return $html;
	}
	
	public function totalizaBeneficiariosPlanoTrabalho( $ptrid ){
		
		$sql = "SELECT DISTINCT
				  sum(ptib.ptbquantidaderural) as rural,
				  sum(ptib.ptbquantidadeurbana) as urbana, 
				  sum(ptib.ptbquantidaderural + ptib.ptbquantidadeurbana) as total
				FROM
				  emenda.ptiniciativa pti INNER JOIN emenda.ptiniciativabeneficiario ptib 
				  ON (pti.ptiid = ptib.ptiid) RIGHT JOIN emenda.iniciativabeneficiario inb
				  ON (ptib.icbid = inb.icbid)
				WHERE
				  inb.icbstatus = 'A' 
				  AND pti.ptrid = ".$ptrid;
		
		$totalben =  $this->db->carregar($sql);
		$totalben = $totalben ? $totalben : array();
		
		$html = "<table class='listagem' width='100%'>";
		
		foreach( $totalben as $totalbenef ){
			$html.="<tr>
						<td align='center' width='60%'></td>
						<td align='center'><b>".$totalbenef['rural']."</b></td>
						<td align='center'><b>".$totalbenef['urbana']."</b></td>
						<td align='center'><b>".$totalbenef['total']."</b></td>
					<tr>";
		}
		$html.= '</table>';
		
		return $html;
	}
	
	public function listaPlanoEspecificacao($ptiid, $pteid, $ptrid, $boLista = false){

		validaSessionPTA( $ptiid );
		validaSessionPTA( $ptrid );
		
		$arData = $this->pegaDataEspecificacao( $ptiid );
		
		$dataini = $arData['min_dataini'];
		$datafim = $arData['max_datafim'];
		
		$html = '<table width="100%" id="tbListaEspecificacao" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		$html.= $this->cabecalhoEspecificacaoIniciativa();
		$html.= '<tr bgcolor="#FCFDDB">';
		$html.= '	<td><div id="combo">'.$this->montaComboEspecificacao( $ptiid ).'</div> <div id="texto" style="display: none"></div> </td>';
		$html.= '	<td valign="middle"><div id="unidade"></div><div id="unidade1"><input type="hidden" name="espkit" id="espkit" value=""></div></td>';
		$html.= '	<td style="text-align: center;">';
		$html.= '		<input id="ptequantidade" class="normal" type="text" title="Quantidade" onblur="MouseBlur(this);calculaTotal(\'\');" 
							onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
							onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" value="0" maxlength="20" size="6" name="ptequantidade"/>
						<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>';
		$html.= '	</td>';
		$html.= '	<td style="text-align: center;">'.campo_texto( 'ptevalorunitario', 'S', 'S', 'Valor Unitário', 18, 18, '[###.]###,##', '','','','','id="ptevalorunitario"','','',"calculaTotal(this.value); this.value=mascaraglobal('[###.]###,##',this.value)").'</td>';
		$html.= '	<td style="text-align: center;"><b><div id="total"></div></b></td>';
		//if( $_SESSION['emenda']['federal'] == 'N' ){
		$html.= '	<td style="text-align: center;">'.campo_texto( 'ptevalorproponente', 'N', 'S', '', 18, 13, '[###.]###,##', '','','','','id="ptevalorproponente"','','',"this.value=mascaraglobal('[###.]###,##',this.value); verificaValorTotal(this.value);").'</td>';

		/*} else {
			$html.= '<input type="hidden" name="ptevalorproponente" id="ptevalorproponente" value="">';
		}*/
		$html.= '	<td style="text-align: center;"><b><div id="concedente"></div></b></td>';
		$html.= '	<td style="text-align: center;"><input type="hidden" value="'.$dataini.'" name="ptedatainicio" id="ptedatainicio" size="12">'.$dataini.'</td>';
		$html.= '	<td style="text-align: center;"><input type="hidden" value="'.$datafim.'" name="ptedatafim" id="ptedatafim" size="12">'.$datafim.'</td>';
		$html.= '	<td style="text-align: center;">';
		$html.= '		<input type="button" value="Salvar" name="btnIncluir" id="btnIncluir" onclick="incluirEspecificacao();">';		
		$html.= '		<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="Cancelar();">';
		$html.= '	</td>';
		$html.= '</tr>';
		$html.= '<tr bgcolor="#FFFFFF" id="trRecurso" style="display: \'\';">';
		$html.= '	<td colspan="10" style="padding-left: 15px;padding-bottom: 25px;padding-top: 10px;"><div id="iniciativaEspecificacaoRecurso" style="display: \'\'">'.$this->iniciativaEspecificacaoRecursoAjax($ptiid, '', $ptrid).'</div></td>';
		$html.= '</tr>';
		$html.= $this->listaEspecificacaoCadastradas( $ptiid, $ptrid, '', $boLista );		
		
		echo $html;
	}
	public function iniciativaEspecificacaoRecursoAjax( $ptiid, $pteid = '', $ptrid=null ){
		if( $ptiid && $ptrid ){
			$arDados = $this->iniciativaEspecificacaoRecurso( $ptiid, $pteid, $ptrid );
		}
		validaSessionPTA( $ptrid );
		$arDados = ( $arDados ? $arDados : array() );
		$html = '<span>
					  Abaixo estão listadas os recursos do PTA que poderão financiar a especificação. Indique o valor a ser utilizado em cada recurso:</span>
				<table id="tblform" class="listagem" width="80%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left">
					<thead>
					<tr>
						<td>&nbsp;</td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Código</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Tipo</strong></td>
						<td align="Center" class="title" width="25%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Autor</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Total da Emenda</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>(-) Já Utilizado</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>(=) Valor Disponível</strong></td>
						<td align="Center" class="title" width="25%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor</strong></td>
					</tr>
					</thead>';
		if($arDados){
			if(sizeof($arDados) == 1){
				$boValor = 'true';
				$html.= '<input type="hidden" name="boPedid" id="boPedid" value="'.$arDados[0]['pedid'].'">';
			}else{
				$boValor = 'false';
			}
			$html.= '<input type="hidden" name="boValor" id="boValor" value="'.$boValor.'">';
			foreach ($arDados as $key => $value) {
				$key % 2 ? $cor = "#dedfde" : $cor = "";
				$pervalorUtilizado = $value['pervalorutilizado'] - $value["pervalor"];
				$value["pervalor"] = number_format($value["pervalor"] , 2, ',', '.');
				
				$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\'" onmouseover="this.bgColor=\'#ffffcc\';">
							<td><img src="/imagens/seta_filho.gif" border="0"></td>
							<td style="text-align: center;">'.$value['emecod'].'</td>
							<td style="text-align: center;">'.$value['tipoemenda'].'</td>
						  	<td style="text-align: center;">'.$value['autnome'].'</td>
						  	<td style="text-align: center;">'.number_format($value['pedvalor'],2,',','.').'</td>
						  	<td style="text-align: center;">'.number_format($pervalorUtilizado,2,',','.').'</td>
						  	<td style="text-align: center;">'.number_format( $value['pedvalor'] - $pervalorUtilizado,2,',','.').'</td>
						  	<td style="text-align: center;">
						  			<input id="pervalor_'.$value['pedid'].'" class="normal" type="text" title="" style="width: 43ex;" onblur="MouseBlur(this); formataValor(this); calculaEmendaEspecificacaoRecurso(); populaHidden(this.value, '.$value['pedid'].'); this.value=mascaraglobal(\'[###.]###,##\',this.value)" 
						  				onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
						  				onkeyup="this.value=mascaraglobal(\'[###.]###,##\',this.value);" value="'.$value["pervalor"].'" maxlength="15" size="41" name="pervalor_'.$value['pedid'].'"/></td>
						  			<input type="hidden" name="pervalor['.$value['pedid'].']" id="pervalor['.$value['pedid'].']" value="">
						  			<input type="hidden" name="emecod['.$key.']" id="emecod['.$key.']" value="'.$value['emecod'].'">
						</tr>';
			}
			$html.= '<tr>
						<td colspan="6" bgcolor="#F5F5F5" style="text-align: right;">Total do concedente:</td>
						<td style="text-align: left;"><span id="concedente2">R$ 0,00</span></td>
					</tr>
					<tr>
						<td colspan="6" bgcolor="#F5F5F5" style="text-align: right;">(-)Total informado:</td>
						<td style="text-align: left;"><span id="informado">R$ 0,00</span></td>
					</tr>
					<tr>
						<td colspan="6" bgcolor="#F5F5F5" style="text-align: right;">(=)Diferença:</td>
						<td style="text-align: left;"><span id="restante">R$ 0,00</span></td>
					</tr>';
		} else{
			$html.= '<table width="80%" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			$html.= '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
			$html.= '</table>';	
		}
		$html.= "</table>";
		
		if( $pteid ){
			echo $html;
		} else {
			return $html;
		}
	}
	public function iniciativaEspecificacaoRecurso( $ptiid, $pteid = '', $ptrid=null ){
		if ( !empty($pteid) ){
			$select = " pr.pervalor, pr.perid,";
			
			$join = " LEFT JOIN emenda.ptiniesprecurso pr
							ON pr.pedid = ped.pedid AND pr.pteid = {$pteid}";
		}
		
		$ptridpai = pegaPaiPTA( $ptrid );
		$filtro =  ( !empty($ptridpai) ? " or pt.ptrid = $ptridpai " : '');
	
		$sql = "SELECT DISTINCT 
				    vede.edeid, 
					vede.emecod,
					(CASE WHEN vede.emetipo = 'E' THEN 'Emenda'
						ELSE 'Complemento' END) as tipoemenda, 
				    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome, 
				    aut.autid, 
				    fup.fupdsc,
					ped.pedvalor,
	                (SELECT SUM(sper.pervalor)
	                 FROM emenda.ptiniesprecurso sper
	                 	inner join emenda.ptemendadetalheentidade pt
                        	on pt.pedid = sper.pedid
	                 WHERE sper.pedid = ped.pedid $filtro) as pervalorutilizado,
				    {$select}
				    ped.pedid
				FROM emenda.planotrabalho ptr
					INNER JOIN emenda.ptemendadetalheentidade  ped
				    	ON ptr.ptrid = ped.ptrid
				    INNER JOIN emenda.v_emendadetalheentidade vede
				    	ON vede.edeid = ped.edeid
				    left JOIN emenda.v_funcionalprogramatica fup
				    	ON (vede.acaid = fup.acaid)
				    INNER JOIN emenda.autor aut
				    	ON aut.autid = vede.autid
				    INNER JOIN emenda.ptiniciativa pti
				    	on pti.ptrid = ptr.ptrid
				    {$join}			    
				WHERE ptr.ptrid = ".$ptrid."
					AND pti.ptiid = $ptiid
				    AND vede.edestatus = 'A'";
				    
		return $this->db->carregar($sql);
	}
	public function listaEspecificacaoCadastradas( $ptiid, $ptrid, $boAjax = false, $boLista = false ){
		
		$acao = "( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('|| ptie.pteid || ', this)\" \" border=0 alt=\"Ir\" title=\"Alterar\">  '  || 
		           '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('|| ptie.pteid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
		
		validaSessionPTA( $ptiid );
		validaSessionPTA( $ptrid );
		
		$sql = "SELECT 
				  	  ".$acao." as acao,
				      e.espnome,
					  e.espkit,
					  e.espunidademedida,
					  ptie.ptiid,
					  ptie.pteid,
					  ptie.iceid,
					  ptie.ptequantidade,
					  ptie.ptevalorunitario,
					  (ptie.ptequantidade * ptie.ptevalorunitario) as total,
					  ptie.ptevalorproponente,
					  ( CASE WHEN ptie.ptevalorproponente is null THEN (ptie.ptequantidade * ptie.ptevalorunitario)
					         ELSE ((ptie.ptequantidade * ptie.ptevalorunitario) - ptie.ptevalorproponente) END) as concedente,
					  to_char(ptie.ptedatainicio, 'DD/MM/YYYY') as dataini,
					  to_char(ptie.ptedatafim, 'DD/MM/YYYY') as datafim
					FROM 
					  emenda.ptiniciativaespecificacao ptie INNER JOIN emenda.iniciativaespecificacao ie ON (ptie.iceid = ie.iceid) 
					  INNER JOIN emenda.especificacao e ON (ie.espid = e.espid)
					  inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
					WHERE 
					    --    ie.icestatus = 'A' AND 
						--    e.espstatus = 'A' AND 
					    ptie.ptestatus = 'A' AND 
					    ptie.ptiid = $ptiid
					ORDER BY
						ptie.pteid";

		$arDados = $this->db->carregar($sql);		
		$totalizadorValorTotal		= 	0;
		$totalizadorValorProponente	=	0;
		$totalizadorValorConcedente	=	0;
		
		$arData = $this->pegaDataEspecificacao( $ptiid );		
		$totalizadorDataInicio	=	$arData["min_dataini"];
		$totalizadorDataFim		=	$arData["max_datafim"];
		
		$html = '';
		if($arDados){
			$html.= ($boAjax ? '<table>' : '');
			$html.= '<tr>
			  			<th bgcolor="#E0E0E0" colspan="10" style="text-align: center"><b>Lista de Especificações</b></th>
			  		</tr>';
			$html.= $this->cabecalhoEspecificacaoIniciativa( $boAjax );
			foreach ($arDados as $key => $value) {
				$key % 2 ? $cor = "#dedfde" : $cor = "";
				
				if($value['espkit'] == 't' && !$boLista){
					$unidade = '<a onclick="AlteraKit('.$value['pteid'].','.$value['iceid'].','."'true'".');" style="cursor: pointer;">'.$value['espunidademedida'].'</a>';
				}else{
					$unidade = $value['espunidademedida'];
				}
			
			$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
			$html.= '	<td><img src="/imagens/mais.gif" border="0" id="img_dimensao_'.$value['pteid'].'" 
							onclick="carregaDetalheEmenda(this.id, \''.$value['pteid'].'\', \'listaEmendaDetalhe_'.$value['pteid'].'\', \'trV_'.$value['pteid'].'\');">
							&nbsp;'.$value['espnome'].'</td>';
			$html.= '	<td>'.$unidade.'</td>';
			$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">'.$value['ptequantidade'].'</td>';
			$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($value['ptevalorunitario'],2,',','.').'</td>';
			$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($value['total'],2,',','.').'</td>';
			//if( $_SESSION['emenda']['federal'] == 'N' ){
				$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($value['ptevalorproponente'],2,',','.').'</td>';
			//}
			$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($value['concedente'],2,',','.').'</td>';
			$html.= '	<td style="text-align: center;">'.$value['dataini'].'</td>';
			$html.= '	<td style="text-align: center;">'.$value['datafim'].'</td>';
			$html.= ($boAjax ? '' : '	<td style="text-align: center;">'.$value['acao'].'</td>');
			$html.= '</tr>';
			$html.= '<tr bgcolor="#FFFFFF" id="trV_'.$value['pteid'].'" style="display: none">';
			$html.= '	<td colspan="10" style="padding-left: 15px;padding-bottom: 25px;padding-top: 10px;"><div id="listaEmendaDetalhe_'.$value['pteid'].'" style="display: none">'.$this->carregaIniciativaEmendaAjax($value['pteid'], $ptrid).'</div></td>';
			$html.= '</tr>';
			
			// Soma os valores para o totalizador
			$totalizadorValorTotal			+= 	$value['total'];
			$totalizadorValorProponente		+=	$value['ptevalorproponente'];
			$totalizadorValorConcedente		+=	$value['concedente'];
			}
			$html.= '<tr>';
			$html.= '<td colspan="4" bgcolor="#cccccc" style="text-align: right;"><b>Totalizador:</b></td>';
			$html.= '<td style="text-align: center;color: rgb(0, 102, 204);"><b>R$ '.number_format($totalizadorValorTotal,2,',','.').'</b></td>';
			//if($_SESSION['emenda']['federal'] == 'N'){
				$html.= '<td style="text-align: center; color: rgb(0, 102, 204);"><b>R$ '.number_format($totalizadorValorProponente,2,',','.').'</b></td>';
			//}
			$html.= '<td style="text-align: center; color: rgb(0, 102, 204);"><b>R$ '.number_format($totalizadorValorConcedente,2,',','.').'</b></td>';
			$html.= '<td style="text-align: center;"><b>'.$totalizadorDataInicio.'</b></td>';
			$html.= '<td style="text-align: center;"><b>'.$totalizadorDataFim.'</b></td>';
			$html.= '</tr>';
			$html.= '</table>';
		
			$html.= '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			$html.= '<tr bgcolor="#ffffff">';
			$html.= '	<td><b>Total de Registros: '.count($arDados).'</b></td>';
			$html.= '	<td></td>';
			$html.= '</tr>';
			$html.= '</table>';
		}else{
			$html.= '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			$html.= '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros</td></tr>';
			$html.= '</table>';
		}
		
		if($boAjax){
			echo $html;
		}else
			return $html;
	}
	
	public function montaComboEspecificacao( $ptiid ){
		
		if( $ptiid ){
			$sql = "SELECT
					e.espnome as descricao,
					ie.iceid as codigo
				FROM
					emenda.iniciativa i
				    inner join emenda.iniciativaespecificacao ie on ie.iniid = i.iniid
				    inner join emenda.especificacao e on e.espid = ie.espid
				    inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
				    inner join emenda.ptiniciativa pti on pti.iniid = i.iniid
				    left join emenda.ptiniciativaespecificacao ptie
				    	on ptie.iceid = ie.iceid
				    	and ptie.ptiid = pti.ptiid
				    	and ptie.ptestatus = 'A'
				WHERE
					pti.ptiid = $ptiid
				    AND ie.icestatus = 'A'
				    AND e.espstatus = 'A'
				    AND i.inistatus = 'A'
				    AND ie.iceid NOT IN (SELECT 
				                            iceid
				                          FROM 
				                            emenda.ptiniciativaespecificacao
				                          WHERE ptestatus = 'A'
				                          	AND ptiid = $ptiid)
				    ORDER BY e.espnome";
			
			$arEspecif = $this->db->carregar( $sql );
		}
		$arEspecif = ( $arEspecif ? $arEspecif : array() );
		
		$html = '<select id="iceid" onchange="montaComboUnidade(this.value)" style="width: 250px;" class="CampoEstilo" title="Especificação da Iniciativa" name="iceid">';
		$html.= '<option value="">-- Selecione um vinculo --</option>';
		foreach ($arEspecif as $valor) {
			$html.= '<option value="'.$valor['codigo'].'" title="'.$valor['descricao'].'">'.$valor['descricao'].'</option>';
		}
		$html.= '</select>';
		
		return $html;
	}
	
public function listaEscolaBeneficiada( $ptrid ){
		
		$sql = "SELECT DISTINCT
					peb.esbid, 
				    ent.entid,
				    ent.entcodent,
				    ent.entnome, 
				    peb.esbquantidadealunos
				FROM
					emenda.ptescolasbeneficiadas peb
				    inner join entidade.entidade ent
				    	on ent.entid = peb.entid
				    inner join entidade.funcaoentidade fent
				        ON fent.entid = ent.entid
				WHERE
					ent.entstatus = 'A'
				    and fent.funid in(3,4,11,12,16,18,56) 
				    and peb.ptrid = $ptrid";
				
		$dados = $this->db->carregar( $sql );	
		$dados = $dados ? $dados : array();
		
		$html = '<input type="hidden" name="ptrid" id="ptrid" value="'.$ptrid.'">
				<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td width="20%" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
							<strong>Código Censo Escolar - INEP</strong></td>
						<td width="50%" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
							<strong>Nome da Escola</strong></td>
						<td width="10%" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
							<strong>Alunos Beneficiados</strong></td>
						<td width="20%" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
							<strong>Ação</strong></td>
					</tr>
				</thead>
					<tr>
						<td>
							<input type="hidden" name="esbid" id="esbid" value="">
							<a href="#" onclick="consultarINEP()"><img src="../imagens/consultar.gif" title="Consultar código INEP" style="cursor:pointer;" border="0"></a>
							<input class="normal" type="text" name="inepID" size="18" id="inepID" value="" style="text-align:center" onkeypress="return somenteNumeros(event);">
						</td>
						<td id="td_escola" align="Center">
							<input type="hidden" name="entid" id="entid" value="">
							<input class="normal" type="text" size="100" name="nmescola" id="nmescola" readonly style="text-align:center" value="" onfocus="buscaEscola(document.formulario.inepID.value);">
						</td>
						<td><input class="normal" type="text" name="esbquantidadealunos" id="esbquantidadealunos" style="text-align:center" value="" onkeypress="return somenteNumeros(event);" onfocus="buscaEscola(document.formulario.inepID.value)"></td>
						<td align="Center">
							<input type="button" name="btSalvar" id="btSalvar" value="Incluir" onclick=\'buscaEscola(document.formulario.inepID.value); salvarEscola();\' />
							<input type="button" name="btCancelar" id="btCancelar" value="Cancelar" onclick=\'CancelaEscola();\'>
						</td>
					</tr>
					<tr>';

		foreach( $dados as $esb ){
			$acao = "<a style=\"cursor:pointer;\" onclick=\"alterar($esb[esbid]);\">
						<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
					</a>
					<a  style=\"cursor:pointer;\" onclick=\"removerEscola($esb[esbid]);\">
						<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
					</a>";
			
			 $html.= "	<tr>
						<td align='center'><b>
								<input type='hidden' name='esbid_".$esb['esbid']."' id='esbid_".$esb['esbid']."' value='".$esb['esbid']."'>
								<input type='hidden' name='entid_".$esb['esbid']."' id='entid_".$esb['esbid']."' value='".$esb['entid']."'>
								<input type='hidden' name='inepID_".$esb['esbid']."' id='inepID_".$esb['esbid']."' value='".$esb['entcodent']."'>
								<input type='hidden' name='entnome_".$esb['esbid']."' id='entnome_".$esb['esbid']."' value='".$esb['entnome']."'>
								<input type='hidden' name='esbquantidadealunos_".$esb['esbid']."' id='esbquantidadealunos_".$esb['esbid']."' value='".$esb['esbquantidadealunos']."'>".
			 					$esb['entcodent']."</b></td>
						<td align='center'>".$esb['entnome']."</td>
						<td align='center'>".$esb['esbquantidadealunos']."</td>
						<td align='center'>".$acao."</td>
					</tr>";	
		}
		$sql = "SELECT count(*) as quant,sum(esbquantidadealunos) as total
				FROM emenda.ptescolasbeneficiadas
				where ptrid=$ptrid";
				
		$dados = $this->db->carregar($sql);
		$dados = $dados ? $dados : array();
		foreach( $dados as $esb ){
			if($esb['quant']>1)
				$rotulo = " Escolas";
			else
				$rotulo = " Escola"; 
			$html.= "	<tr bgcolor='#c0c0c0'>
						<td align='center'><b> Totais</b></td>
						<td align='center'><b>".$esb['quant'].$rotulo."</b></td>
						<td align='center'><b>".$esb['total']."</b></td>
						<td align='center'></td>
					</tr>";	
		}
		$html.= "</table>";
		
		echo $html;
	}
	
	public function montaListaCronogramaDesembolso( $ptrid, $obCronograma, $boLista = false ){		
		$iniciativas = $obCronograma->carregaIniciativaCronograma();
		$cronogramaExecucao = $obCronograma->verificaIniciativaVinculadaEspecificacao( $boLista );
		$prdminuta = 'P';		
		?>
		<table align="center" class="Tabela" cellpadding="10" cellspacing="1">
		 <tbody>
			<tr>
				<td width="100%" style="text-align: left;" class="SubTituloEsquerda">Cronograma de Execução de Desembolso</td>
			</tr>
			<tr>
				<td width="100%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
					<table style="border-color:#ffffff;" align="center" class="Tabela" cellpadding="5" cellspacing="0">
					 <tbody>
						<tr>
							<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">Mês/Ano Inicial:</td>
							<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=(!empty($cronogramaExecucao[0]['inicial']) ? $cronogramaExecucao[0]['inicial'] : 'NULL');?></td>
						</tr>
						<tr>
							<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">Mês/Ano Final:</td>
							<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=(!empty($cronogramaExecucao[0]['final']) ? $cronogramaExecucao[0]['final'] : 'NULL');?></td>
						</tr>
					 </tbody>
					</table>
				</td>
			</tr>
	<tr>
		<td width="100%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<table style="border-color:#eeeeee;" align="center" class="Tabela" cellpadding="5" cellspacing="0">
			 <tbody>
			 	<?
					$sql = "SELECT 
								to_char(prddata, 'MM/YYYY') as data,
								prdid
							FROM 
								emenda.ptparceladesembolso 
							WHERE 
								ptrid = ".($ptrid)." 
								AND prdtipo = 'C'
								AND prdminuta = '$prdminuta'
							ORDER BY
								prddata ASC";
								
					$parcelasConcedente = $this->db->carregar($sql);
					$parcelasConcedente = $parcelasConcedente ? $parcelasConcedente : array();
			 	?>
			 	<tr>
			 		<td width="100%" colspan="5" style="text-align: center;" class="SubTituloEsquerda">DESEMBOLSO DO CONCEDENTE</td>
			 	</tr>
			 	<input type="hidden" id="num_parcelas_concedente" value="<?=count($parcelasConcedente)?>" />
				<tr>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Iniciativa</b></td>
					<?
					if( !empty($parcelasConcedente) ){
						foreach ($parcelasConcedente as $key => $v) {
							$excluirC = '&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - '.$v["data"];
							echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$excluirC.'</b></td>';
						}
					} else {
						$excluirC = '&nbsp;&nbsp;<b>1ª Parcela';
						echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$excluirC.'</b></td>';
					}
					?>
					
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Total já informado para a iniciativa</b></td>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Restante</b></td>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Valor da Iniciativa (concedente)</b></td>
				</tr>
				<?
					$arrTotalParcelas 		= array();
					$totalTotalParcelas 	= 0;
					$totalRestante 			= 0;
					$totalValorIniciativa	= 0;
					
					for($i=0; $i<count($iniciativas); $i++) {
						echo '<tr>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$iniciativas[$i]['ininome'].'</td>';
						
						$valorConcedente = $obCronograma->carregaParcelaCronograma($iniciativas[$i]["ptiid"], $prdminuta, 'C');
						
						if( !$boLista )
							echo '<script> id_ptiid.push('.$valorConcedente[0]["ptiid"].'); </script>';
						
						$totalParcelas = 0;
						for($k=0; $k<count($valorConcedente); $k++) {
							$name = 'privalor['.$valorConcedente[$k]["priid"].']';
							$prdtipo = 'prdtipo['.$valorConcedente[$k]["priid"].']';
							if( $boLista ){
								$campoConcedente = number_format($valorConcedente[$k]["privalor"],2,",","."); 
							} else {
								$campoConcedente = '<input class="normal" id="'.$valorConcedente[$k]["ptiid"].'_C_'.$k.'" name="'.$name.'" type="text" maxlength="13" size="15" value="'.number_format($valorConcedente[$k]["privalor"],2,",",".").'" onkeyup="this.value = mascaraglobal(\'###.###.###,##\',this.value);" />';
							}
							echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
									R$ '.$campoConcedente.'
									<input type="hidden" name="'.$prdtipo.'" id="prdtipo" value="C">
								  </td>';
							
							$totalParcelas 			+= $valorConcedente[$k]["privalor"];
							$arrTotalParcelas[$k]	+= $valorConcedente[$k]["privalor"];
							
							$restante = ($valorConcedente[$k]["ptivalorconcedente"] - $totalParcelas);

							$totalValorIniciativa	+= $valorConcedente[$k]["ptivalorconcedente"];
							
							$ptivalorconcedente		= $valorConcedente[$k]["ptivalorconcedente"];
							$ptiid					= $valorConcedente[$k]["ptiid"];
						}
						$totalvalorconcedente	+= $ptivalorconcedente;
						$totalTotalParcelas 	+= $totalParcelas;
						$totalRestante			+= $restante;
						
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($totalParcelas,2,",",".").'</td>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($restante,2,",",".").'</td>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.($ptivalorconcedente ? number_format($ptivalorconcedente,2,",",".") : '0,00').'</td>';
						echo '</tr>';
						echo '<input type="hidden" id="total_C_'.$ptiid.'" value="'.(($ptivalorconcedente) ? $ptivalorconcedente : 0).'" />';
					}
					
					//total
					echo '<tr>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: right;" class="SubTituloDireita"><b>Total:</b></td>';
					for($i=0; $i<count($arrTotalParcelas); $i++) {
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($arrTotalParcelas[$i],2,",",".").'</b></td>';
					}
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalTotalParcelas,2,",",".").'</b></td>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalRestante,2,",",".").'</b></td>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalvalorconcedente,2,",",".").'</b></td>';
					echo '</tr>';
				?>
			 </tbody>
			</table>
			<br />
			<table style="border-color:#eeeeee;" align="center" class="Tabela" cellpadding="5" cellspacing="0">
			 <tbody>
			 	<?
					$sql = "SELECT 
								to_char(prddata, 'MM/YYYY') as data,
								prdid
							FROM 
								emenda.ptparceladesembolso 
							WHERE 
								ptrid = ".$ptrid." 
								AND prdtipo = 'P'
								AND prdminuta = '$prdminuta'
							ORDER BY
								prddata ASC";
					$parcelasProponente = $this->db->carregar($sql);
					$parcelasProponente = $parcelasProponente ? $parcelasProponente : array();
			 	?>
			 	<tr>
			 		<td width="100%" colspan="5" style="text-align: center;" class="SubTituloEsquerda">DESEMBOLSO DO PROPONENTE</td>
			 	</tr>
			 	<input type="hidden" id="num_parcelas_proponente" value="<?=count($parcelasProponente)?>" />
				<tr>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Iniciativa</b></td>
					<?
					if( !empty($parcelasProponente) ){
						foreach ($parcelasProponente as $key => $v) {
							$excluirP = '&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - '.$v["data"];
							echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$excluirP.'</b></td>';
						}
					} else {
						$excluirP = '&nbsp;&nbsp;<b>1ª Parcela';
						echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$excluirP.'</b></td>';
					}
					?>					
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Total já informado para a iniciativa</b></td>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Restante</b></td>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Valor da Iniciativa (proponente)</b></td>
				</tr>
				<?
					$arrTotalParcelas 		= array();
					$totalTotalParcelas 	= 0;
					$totalRestante 			= 0;
					$totalValorIniciativa	= 0;
					
					for($i=0; $i<count($iniciativas); $i++) {
						echo '<tr>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$iniciativas[$i]['ininome'].'</td>';

						$valorProponente = $obCronograma->carregaParcelaCronograma($iniciativas[$i]["ptiid"], $prdminuta, 'P');
						
						$totalParcelas = 0;
						for($k=0; $k<count($valorProponente); $k++) {
							if( $boLista ){
								$campoProponente = number_format($valorProponente[$k]["privalor"],2,",","."); 
							} else {
								$campoProponente = '<input class="normal" id="'.$valorProponente[$k]["ptiid"].'_P_'.$k.'" name="privalor['.$valorProponente[$k]["priid"].']" type="text" maxlength="13" size="15" value="'.number_format($valorProponente[$k]["privalor"],2,",",".").'" onkeyup="this.value = mascaraglobal(\'###.###.###,##\',this.value);" />';
							}
							echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
									R$ '.$campoProponente.'
									<input type="hidden" name="prdtipo['.$valorProponente[$k]["priid"].']" id="prdtipo" value="P">
								  </td>';
							
							$totalParcelas 			+= $valorProponente[$k]["privalor"];
							$arrTotalParcelas[$k]	+= $valorProponente[$k]["privalor"];
							
							$restante = ($valorProponente[$k]["ptivalorproponente"] - $totalParcelas);
							
							$totalValorIniciativa	+= $valorProponente[$k]["ptivalorproponente"];
							
							$ptivalorproponente = $valorProponente[$k]["ptivalorproponente"];
							$ptiid				= $valorProponente[$k]["ptiid"];
						}
						$totalvalorproponente	+= $ptivalorproponente;
						$totalTotalParcelas 	+= $totalParcelas;
						$totalRestante			+= $restante;
						
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($totalParcelas,2,",",".").'</td>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($restante,2,",",".").'</td>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($ptivalorproponente,2,",",".").'</td>';
						echo '</tr>';
						echo '<input type="hidden" id="total_P_'.$ptiid.'" value="'.(($ptivalorproponente) ? $ptivalorproponente : 0).'" />';
					}
					
					//total
					echo '<tr>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: right;" class="SubTituloDireita"><b>Total:</b></td>';
					for($i=0; $i<count($arrTotalParcelas); $i++) {
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($arrTotalParcelas[$i],2,",",".").'</b></td>';
					}
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalTotalParcelas,2,",",".").'</b></td>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalRestante,2,",",".").'</b></td>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalvalorproponente,2,",",".").'</b></td>';
					echo '</tr>';
				?>
			 </tbody>
			</table>
		</td>
	</tr>
	<? if( !$boLista ){ ?>
	<tr>
		<td width="100" style="text-align: center" class="SubTituloEsquerda">
				<input type="button" value="Salvar Cronograma" id="bt_salvar" onclick="salvarCronograma();"/>
				<input type="button" value="Fechar" id="bt_fechar" onclick="window.close();" >
				
			</td>
	</tr>
	<?} ?>
 </tbody>
</table><?
	}
}
?>