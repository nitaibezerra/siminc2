<?php 
set_time_limit(0);
ini_set('memory_limit', '1024M');

$_SESSION['emenda']['relcoluna'] = $_POST['coluna'] ? $_POST['coluna'] : $_SESSION['emenda']['relcoluna'];

//include  APPRAIZ."includes/cabecalho.inc";
echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	 <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
print '<br/>
			<form action="" method="post" name="formulario" id="formulario">
			<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
			<table  align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td style="height: 20px;"></td>
				</tr>
				<tr>
					<td style="text-align: center;" class="div_rolagem">
						<input type="button" name="excel" value="Gerar Excel" onclick="exportarExcel();">
					</td>
				</tr>
			</table>
			</form>
			<script type="text/javascript">				
				function exportarExcel(){
					document.getElementById(\'exporta\').value = "true";
					document.getElementById(\'formulario\').submit();
				}
				//$(\'loader-container\').show();
			</script>';
			

$arrColunas = array();
if($_SESSION['emenda']['relcoluna']){
    foreach ($_SESSION['emenda']['relcoluna'] as $coluna) {
            switch ($coluna) {
                    case 'pactuado':
                            array_push( $arrColunas, array( 'cabecalho' => 'pactuado', 
                                                                                            'colunaPrincipal' => 'pti.ptivalortotal as global', 
                                                                                            'coluna' => 'sum(global) as global',
                                                                                            'gruopPrincipal' => 'pti.ptivalortotal',
                                                                                            'gruop' => '')
                                                                                            );

                            array_push( $arrColunas, array( 'cabecalho' => 'pactuado', 
                                                                                            'colunaPrincipal' => '(ptr.ptrvalorproponente) as ptrvalorconcedente', 
                                                                                            'coluna' => 'sum(ptrvalorconcedente) as ptrvalorconcedente',
                                                                                            'gruopPrincipal' => 'ptr.ptrvalorproponente',
                                                                                            'gruop' => '')
                                                                                            );

                    break;
                    case 'repassado':
                            array_push( $arrColunas, array( 'cabecalho' => 'repassado', 
                                                                                            'colunaPrincipal' => 'ord.orbvalorpagamento as valorpago', 
                                                                                            'coluna' => 'sum(valorpago) as valorpago',
                                                                                            'gruopPrincipal' => 'ord.orbvalorpagamento',
                                                                                            'gruop' => '')
                                                                                            );

                            array_push( $arrColunas, array( 'cabecalho' => 'repassado', 
                                                                                            'colunaPrincipal' => '(pti.ptivalortotal - ptr.ptrvalorproponente - ord.orbvalorpagamento) as acumulado', 
                                                                                            'coluna' => 'sum(acumulado) as acumulado',
                                                                                            'gruopPrincipal' => 'pti.ptivalortotal',
                                                                                            'gruop' => '')
                                                                                            );

                    break;
                    case 'vigencia':
                            array_push( $arrColunas, array( 'cabecalho' => 'vigencia', 
                                                                                            "colunaPrincipal" => "to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as vigdatainicio", 
                                                                                            'coluna' => 'vigdatainicio',
                                                                                            'gruopPrincipal' => 'ptv.vigdatainicio',
                                                                                            'gruop' => 'vigdatainicio')
                                                                                            );

                            array_push( $arrColunas, array( 'cabecalho' => 'vigencia', 
                                                                                            "colunaPrincipal" => "to_char(ptv.vigdatafim, 'DD/MM/YYYY') as vigdatafim", 
                                                                                            'coluna' => 'vigdatafim',
                                                                                            'gruopPrincipal' => 'ptv.vigdatafim',
                                                                                            'gruop' => 'vigdatafim')
                                                                                            );
                    break;
                    case 'mapcod':
                            array_push( $arrColunas, array( 'cabecalho' => 'modalidade', 
                                                                                            "colunaPrincipal" => "vede.mapcod ||' - '|| mod.mapdsc as modalidade", 
                                                                                            'coluna' => 'modalidade',
                                                                                            'gruopPrincipal' => 'vede.mapcod, mod.mapdsc',
                                                                                            'gruop' => 'modalidade')
                                                                                            );
                    break;
                    case 'ptrnumconvenio':
                            array_push( $arrColunas, array( 'cabecalho' => 'conveniofnde', 
                                                                                            "colunaPrincipal" => "ptr.ptrnumconvenio", 
                                                                                            'coluna' => 'ptrnumconvenio',
                                                                                            'gruopPrincipal' => 'ptr.ptrnumconvenio',
                                                                                            'gruop' => 'ptrnumconvenio')
                                                                                            );
                    break;
                    case 'bennome':
                            array_push( $arrColunas, array( 'cabecalho' => 'beneficiario', 
                                                                                            "colunaPrincipal" => "", 
                                                                                            'coluna' => '',
                                                                                            'gruopPrincipal' => '',
                                                                                            'gruop' => '')
                                                                                            );
                    break;
                    case 'enbcnpj':
                            array_push( $arrColunas, array( 'cabecalho' => 'cnpjconvenente', 
                                                                                            "colunaPrincipal" => "enb.enbcnpj", 
                                                                                            'coluna' => 'enbcnpj',
                                                                                            'gruopPrincipal' => 'enb.enbcnpj',
                                                                                            'gruop' => 'enbcnpj')
                                                                                            );
                    break;
                    case 'pmcnumconveniosiafi':
                            array_push( $arrColunas, array( 'cabecalho' => 'conveniosiaf', 
                                                                                            "colunaPrincipal" => "ptm.pmcnumconveniosiafi", 
                                                                                            'coluna' => 'pmcnumconveniosiafi',
                                                                                            'gruopPrincipal' => 'ptm.pmcnumconveniosiafi',
                                                                                            'gruop' => 'pmcnumconveniosiafi')
                                                                                            );
                    break;
            }
    }
}

$arColuna = array();
$arCabecalho = array();
$arColunaPrincipal = array();
$arGruopPrincipal = array();
$arGruop = array();

foreach ($arrColunas as $dados) {
	if( $dados['coluna'] ) array_push( $arColuna, $dados['coluna'] );
	if( $dados['colunaPrincipal'] ) array_push( $arColunaPrincipal, $dados['colunaPrincipal'] );
	if( $dados['cabecalho'] ) array_push( $arCabecalho, $dados['cabecalho'] );
	if( $dados['gruopPrincipal'] ) array_push( $arGruopPrincipal, $dados['gruopPrincipal'] );
	if( $dados['gruop'] ) array_push( $arGruop, $dados['gruop'] );
}

if($arColuna){
    $sql = "select ptrcod,
            ptrid,
            ".implode(', ', $arColuna)."
    from (SELECT
                            ptr.ptrcod,
                        ptr.ptrid,
                        ".implode(', ', $arColunaPrincipal)."
                    FROM
                            emenda.planotrabalho ptr
                        inner join emenda.ptemendadetalheentidade pte on pte.ptrid = ptr.ptrid
                        INNER JOIN emenda.v_emendadetalheentidade vede on vede.edeid = pte.edeid
                        inner join public.modalidadeaplicacao mod on mod.mapcod = vede.mapcod
                        LEFT JOIN 
                          (SELECT 
                              ptrid,
                              sum(ptivalortotal) as ptivalortotal,
                              ptiid
                           FROM emenda.v_ptiniciativa 
                           GROUP BY ptrid, ptiid) pti ON pti.ptrid = ptr.ptrid
                        inner join emenda.execucaofinanceira exf on exf.ptrid = ptr.ptrid and exf.pedid = pte.pedid
                        left join emenda.ptvigencia ptv on ptv.ptrid = ptr.ptrid and ptv.vigstatus = 'A'
                        left join emenda.ordembancaria ord on ord.exfid = exf.exfid and ord.spgcodigo = '2'
                inner join emenda.ptminutaconvenio ptm on ptm.ptrid = ptr.ptrid
                inner join emenda.ptpublicacao ptp on ptp.pmcid = ptm.pmcid
                inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid
                    WHERE
                            ptr.ptrstatus = 'A'
                        and vede.edestatus = 'A'
                        and exf.exfstatus = 'A'
                and ptm.pmcstatus = 'A'
                and ptp.pubstatus = 'A'
                and ptr.ptrid not IN (SELECT tt.ptrid FROM emenda.planotrabalho tt WHERE tt.ptridpai = ptr.ptrid and tt.ptrstatus = 'A')
                and ptr.ptrexercicio = '{$_SESSION['exercicio']}'
                and exf.exfnumempenhooriginal <> '' and exf.exfnumempenhooriginal is not null
                and ptr.ptrnumconvenio is not null and ptr.ptrnumconvenio <> ''
                and ptm.pmcnumconveniosiafi is not null and ptm.pmcnumconveniosiafi <> ''
                and ptp.pubdatapublicacao is not null
                    GROUP BY
                            ptr.ptrcod,
                        ptr.ptrid,
                            ".implode(', ', $arGruopPrincipal)."
                     ORDER BY
                            ptr.ptrcod) as foo
                     group by 
                            ptrcod, ptrid,".implode(', ', $arGruop)."";
    $arDados = $db->carregar( $sql );
}
$arDados = $arDados ? $arDados : array();

$arRegistro = array();
$arValue = array();

foreach ($arDados as $key => $v) {
	
	
	if( in_array( 'modalidade', $arCabecalho) ) $arValue['modalidade'] = $v['modalidade']; 
	if( in_array( 'conveniofnde', $arCabecalho) ) $arValue['ptrnumconvenio'] = $v['ptrnumconvenio']; 
	if( in_array( 'conveniosiaf', $arCabecalho) ) $arValue['pmcnumconveniosiafi'] = $v['pmcnumconveniosiafi'];
	
	if( in_array( 'beneficiario', $arCabecalho) ) {
			$sql = "SELECT
				ben.bennome
			FROM 
				emenda.ptiniciativa pt
				inner join emenda.ptiniciativabeneficiario ptib on ptib.ptiid = pt.ptiid
			    inner join emenda.iniciativabeneficiario ibe on ibe.icbid = ptib.icbid
			    inner join emenda.beneficiario ben on ben.benid = ibe.benid
	    	WHERE
	    		 pt.ptrid = ".$v['ptrid']."
	    		 and ben.benstatus = 'A'
    			 and ibe.icbstatus = 'A'";
			$arBenNome = $db->carregarColuna( $sql );
			$arBenNome = $arBenNome ? $arBenNome : array();
			
			$arValue['beneficiario'] = implode( '<br>', $arBenNome );
	}
	if( in_array( 'cnpjconvenente', $arCabecalho) ) $arValue['enbcnpj'] = $v['enbcnpj'];
	
	if( in_array( 'pactuado', $arCabecalho) ){
		$arValue['valor'] = number_format( $v['global'], 2, ',', '.' );
		$arValue['ptrvalorconcedente'] = number_format( $v['ptrvalorconcedente'], 2, ',', '.' );
	}
	if( in_array( 'repassado', $arCabecalho) ){
		$acumulado = ((float)$v['global'] - (float)$v['ptrvalorconcedente']) - (float)$v['valorpago'];
		$arValue['valorpago'] = number_format( $v['valorpago'], 2, ',', '.' );
		$arValue['acumulado'] = number_format( $acumulado, 2, ',', '.' );
	}
	if( in_array( 'vigencia', $arCabecalho) ){
		$arValue['vigdatainicio'] = $v['vigdatainicio'];
		$arValue['vigdatafim'] = $v['vigdatafim'];
	}
	
	$arRegistro[$key] = $arValue;
}

print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
print '<tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" class="subtitulocentro" >Informações sobre as Transferências</td></tr></table>';
$cabecalho = array();
?>
<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
	<thead>
	<tr>
		<?if( in_array( 'modalidade', $arCabecalho ) ): 
			array_push( $cabecalho, 'Modalidade' );?>
		<td bgcolor="" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title" rowspan="2"><strong>Modalidade</strong></td>
		<?endif;
		if( in_array( 'conveniofnde', $arCabecalho ) ): 
			array_push( $cabecalho, 'Nº do Convênio do FNDE' );?>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title" rowspan="2"><strong>Nº do Convênio do FNDE</strong></td>
		<?endif;
		if( in_array( 'conveniosiaf', $arCabecalho ) ): 
			array_push( $cabecalho, 'Nº do Convênio SIAF' );?>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title" rowspan="2"><strong>Nº do Convênio SIAF</strong></td>
		<?endif;
		if( in_array( 'beneficiario', $arCabecalho ) ): 
			array_push( $cabecalho, 'Beneficiário' );?>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title" rowspan="2"><strong>Beneficiário</strong></td>
		<?endif;
		if( in_array( 'cnpjconvenente', $arCabecalho ) ): 
			array_push( $cabecalho, 'CNPJ do Convenente' );?>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title" rowspan="2"><strong>CNPJ do Convenente</strong></td>
		<?endif;
		if( in_array( 'pactuado', $arCabecalho ) ): 
			array_push( $cabecalho, 'Global' );
			array_push( $cabecalho, 'Contrapartida' );?>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title" colspan="2"><strong>Valores Pactuados</strong></td>
		<?endif;
		if( in_array( 'repassado', $arCabecalho ) ): 
			array_push( $cabecalho, 'No exercício/ANO/Pago' );
			array_push( $cabecalho, 'Acumulado até exercício/Não Pago' );?>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title" colspan="2"><strong>Valores Repassados</strong></td>
		<?endif;
		if( in_array( 'vigencia', $arCabecalho ) ): 
			array_push( $cabecalho, 'Vigência Inicio' );
			array_push( $cabecalho, 'Vigência Fim' );?>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title" colspan="2"><strong>Vigência</strong></td>
		<?endif;?>
	</tr>
	<?if( in_array( 'vigencia', $arCabecalho ) || in_array( 'repassado', $arCabecalho ) || in_array( 'pactuado', $arCabecalho ) ): ?>
	<tr>
		<?if( in_array( 'pactuado', $arCabecalho ) ): ?>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title"><strong>Global</strong></td>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title"><strong>Contrapartida</strong></td>
		<?endif;
		if( in_array( 'repassado', $arCabecalho ) ): ?>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title"><strong>No exercício/ANO/Pago</strong></td>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title"><strong>Acumulado até exercício/Não Pago</strong></td>
		<?endif;
		if( in_array( 'vigencia', $arCabecalho ) ): ?>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title"><strong>Início</strong></td>
		<td align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
			style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
			class="title"><strong>Fim</strong></td>
		<?endif;?>
	</tr>
	<?endif; ?>
	</thead>
	<?	
	if( $arDados ){
		$html = '';
		foreach ($arRegistro as $key => $v) {
			$key % 2 ? $cor = "#F0F0F0" : $cor = "";
						
			$html.= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
						if(in_array( 'modalidade', $arCabecalho )) $html .= '<td>'.$v['modalidade'].'</td>';
						if(in_array( 'conveniofnde', $arCabecalho )) $html .= '<td>'.$v['ptrnumconvenio'].'</td>';
						if(in_array( 'conveniosiaf', $arCabecalho )) $html .= '<td>'.$v['pmcnumconveniosiafi'].'</td>';
						if(in_array( 'beneficiario', $arCabecalho )) $html .= '<td>'.$v['beneficiario'].'</td>';
						if(in_array( 'cnpjconvenente', $arCabecalho )) $html .= '<td>'.formatar_cnpj($v['enbcnpj']).'</td>';
						if(in_array( 'pactuado', $arCabecalho )) $html .= '<td>'.$v['valor'].'</td>';
						if(in_array( 'pactuado', $arCabecalho )) $html .= '<td>'.$v['ptrvalorconcedente'].'</td>';
						if(in_array( 'repassado', $arCabecalho )) $html .= '<td>'.$v['valorpago'].'</td>';
						if(in_array( 'repassado', $arCabecalho )) $html .= '<td>'.$v['acumulado'].'</td>';
						if(in_array( 'vigencia', $arCabecalho )) $html .= '<td>'.$v['vigdatainicio'].'</td>';
						if(in_array( 'vigencia', $arCabecalho )) $html .= '<td>'.$v['vigdatafim'].'</td>';
			$html.= '</tr>';
		}
		echo $html;
		?>
	</table>
	
	<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
		<tbody>
		<tr bgcolor="#ffffff">
			<td><b>Total de Registros: <?=sizeof( $arDados ); ?></b></td>
		</tr>
		</tbody>
	</table>
<?} else {?>

<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
	<tbody>
	<tr>
		<td align="center" style="color: rgb(204, 0, 0);">Não foram encontrados Registros.</td>
	</tr>
	</tbody>
</table>
<?} 

if($_POST['exporta'] == "true"){
	global $db;
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	
	
	/*foreach ($arCabecalho as $cab) {
		switch ($cab) {
			case 'modalidade': array_push( $cabecalho, 'Modalidade' ); break;
			case 'conveniofnde': array_push( $cabecalho, 'Nº do Convênio do FNDE' );break;
			case 'conveniosiaf': array_push( $cabecalho, 'Nº do Convênio SIAF' );break;
			case 'beneficiario': array_push( $cabecalho, 'Beneficiário' );break;
			case 'cnpjconvenente': array_push( $cabecalho, 'CNPJ do Convenente' );break;
			case 'pactuado': array_push( $cabecalho, 'Global' );break;
			case 'pactuado': array_push( $cabecalho, 'Contrapartida' );break;
			case 'repassado': array_push( $cabecalho, 'No exercício/ANO/Pago' );break;
			case 'repassado': array_push( $cabecalho, 'Acumulado até exercício/Não Pago' );break;
			case 'vigencia': array_push( $cabecalho, 'Vigência Inicio' );break;
			case 'vigencia': array_push( $cabecalho, 'Vigência Fim' );break;
		}
		
	}*/
	
	//$cabecalho = array('Modalidade', 'Nº do instrumento', 'Beneficiário', 'Global', 'Contrapartida', 'No exercício/ANO/Pago', 'Acumulado até exercício/Não Pago', 'Vigência Inicio', 'Vigência Fim');
	$db->sql_to_excel($arRegistro, 'relInforTransferencia', $cabecalho);
	exit;
	$exporta = "false";
}
?>