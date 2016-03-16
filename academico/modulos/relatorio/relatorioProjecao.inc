<?
ini_set('memory_limit', '512M');

$campos  = array( 'curso' => array(
							'codigo'    => 'curso',
							'descricao' => 'Curso'
						 	),
				 'modalidade' => array(
							'codigo'    => 'modalidade',
							'descricao' => 'Modalidade'
						 	),
				 'unideixoadeorc' => array(
							'codigo'    => 'unideixoadeorc',
							'descricao' => 'Eixo'
						 	),
				 'semestre' => array(
							'codigo'    => 'semestre',
							'descricao' => 'Semestre'
						 	),
				 'vagas' => array(
							'codigo'    => 'vagas',
							'descricao' => 'Vagas'
						 	),
				 'qtdturmas' => array(
							'codigo'    => 'qtdturmas',
							'descricao' => 'Quantidade de Turmas'
						 	),
				 'turnom' => array(
							'codigo'    => 'turnom',
							'descricao' => 'Turno Matutino'
						 	),
				 'turnov' => array(
							'codigo'    => 'turnov',
							'descricao' => 'Turno Vespertino'
						 	),
				 'turnon' => array(
							'codigo'    => 'turnon',
							'descricao' => 'Turno Noturno'
						 	),
				 'cargahr' => array(
							'codigo'    => 'cargahr',
							'descricao' => 'Carga Horária'
						 	),
				 'tpcurso' => array(
							'codigo'    => 'tpcurso',
							'descricao' => 'Tipo de Curso'
						 	),
				 'qtdsemaula' => array(
							'codigo'    => 'qtdsemaula',
							'descricao' => 'Semestres de Aula'
							),
				 'qtdsemestagio' => array(
							'codigo'    => 'qtdsemestagio',
							'descricao' => 'Semestre de Estágio'
						 	),
				 'anoprimoferta' => array(
							'codigo'    => 'anoprimoferta',
							'descricao' => 'Ano da Primeira Oferta'
						 	),
				 'anoultoferta' => array(
							'codigo'    => 'anoultoferta',
							'descricao' => 'Ano da Ultima Oferta'
						 	),
				 'entnome' => array(
							'codigo'    => 'entnome',
							'descricao' => 'Instituição'
							),
				 'campnome' => array(
							'codigo'    => 'campnome',
							'descricao' => 'Campus'
						    ) 
				 );
				 
$origem  = array( 'curso' => array(
							'codigo'    => 'curso',
							'descricao' => 'Curso'
						 	),
				 'modalidade' => array(
							'codigo'    => 'modalidade',
							'descricao' => 'Modalidade'
						 	),
				 'eixo' => array(
							'codigo'    => 'unideixoadeorc',
							'descricao' => 'Eixo'
						 	),
				 'semestre' => array(
							'codigo'    => 'semestre',
							'descricao' => 'Semestre'
						 	),
				 'vagas' => array(
							'codigo'    => 'vagas',
							'descricao' => 'Vagas'
						 	),
				 'qtdturmas' => array(
							'codigo'    => 'qtdturmas',
							'descricao' => 'Quantidade de Turmas'
						 	),
				 'turnom' => array(
							'codigo'    => 'turnom',
							'descricao' => 'Turno Matutino'
						 	),
				 'turnov' => array(
							'codigo'    => 'turnov',
							'descricao' => 'Turno Vespertino'
						 	),
				 'turnon' => array(
							'codigo'    => 'turnon',
							'descricao' => 'Turno Noturno'
						 	),
				 'cargahr' => array(
							'codigo'    => 'cargahr',
							'descricao' => 'Carga Horária'
						 	),
				 'tpcurso' => array(
							'codigo'    => 'tpcurso',
							'descricao' => 'Tipo de Curso'
						 	),
				 'qtdsemaula' => array(
							'codigo'    => 'qtdsemaula',
							'descricao' => 'Semestres de Aula'
							),
				 'qtdsemestagio' => array(
							'codigo'    => 'qtdsemestagio',
							'descricao' => 'Semestre de Estágio'
						 	),
				 'anoprimoferta' => array(
							'codigo'    => 'anoprimoferta',
							'descricao' => 'Ano da Primeira Oferta'
						 	),
				 'anoultoferta' => array(
							'codigo'    => 'anoultoferta',
							'descricao' => 'Ano da Ultima Oferta'
						 	)
				 );
$destino = array('entnome' => array(
							'codigo'    => 'entnome',
							'descricao' => 'Instituição'
							),
				 'campnome' => array(
							'codigo'    => 'campnome',
							'descricao' => 'Campus'
						    ),
				 'curso' => array(
							'codigo'    => 'curso',
							'descricao' => 'Curso'
						    ) 
				 );

function mostrar( $anoInicio = '', $anoFim = '', $filtroSQL = false ) {
	
	global $db, $destino, $campos;
	
	if($filtroSQL[0]){
		$where = 'WHERE 0=0';
		foreach($filtroSQL as $filtro){
			$where .= ' AND '.$filtro;
		}
	}	
	
	if( $anoInicio != '' && $anoFim != '' ){
		$referencia = montaProjecao();
		$periodoReferencia = montaPeriodoProjecao( $anoInicio, $anoFim, $referencia );
	}
	
	$sql =  "SELECT DISTINCT
				entinst.entnome as entnome, 
				entinst.entid as intid, 
				entcampus.entnome as campnome, 
				entcampus.entid as campid, 
				ofc.ofcnomecurso as curso, 
				mod.modnome as modalidade, 
				eix.eixnome as unideixoadeorc, 
				ofc.ofcsemestre as semestre, 
				ofc.ofcnumvagas as vagas, --
				ofc.ofcnumturmas as qtdturmas, -- 
				ofc.ofcturnom as turnom, 
				ofc.ofcturnot as turnov, 
				ofc.ofcturnon as turnon, 
				ofc.ofccargahoraria as cargahr, --
				ofc.ofcorcamento, 
				CASE WHEN ofc.ofcpresencial is true
					THEN 'Presencial'
					ELSE 'A Distância'
				END as tpcurso, 
				ofc.ofcsemestresaula as qtdsemaula, --
				ofc.ofcsemestresestagio as qtdsemestagio,--
				ofc.ofcanoprimeiraoferta as anoprimoferta, --
				ofc.ofcanoultimaoferta as anoultoferta, --
				mod.nivid as nivid --
			 FROM academico.ofertacursos ofc
			 INNER JOIN entidade.entidade     entcampus ON entcampus.entid = ofc.entidcampus
			 INNER JOIN entidade.funcaoentidade      fn ON entcampus.entid = fn.entid and fn.funid = 17 and entcampus.entstatus = 'A'
			 INNER JOIN entidade.funentassoc          a ON a.fueid = fn.fueid
			 INNER JOIN entidade.entidade       entinst ON entinst.entid = a.entid 
			 INNER JOIN entidade.funcaoentidade     fn2 ON entinst.entid = fn2.entid and fn2.funid = 11 and entinst.entstatus = 'A'
			 LEFT JOIN academico.catalogomodalidade mod ON mod.modid = ofc.modid
			 LEFT JOIN academico.catalogoeixo 	    eix ON eix.eixid = ofc.eixid
			 {$where}
			 ORDER BY 
			 	entinst.entnome, entcampus.entnome, ofc.ofcnomecurso";
	
	$dadoscsv = $db->carregar($sql);

	if($dadoscsv[0]) {
		
		$colspan = false;
		$rowspan = false;
		if($_REQUEST['agrupador']) {
			foreach($_REQUEST['agrupador'] as $col) {
				if(!$colspan) $colspan = count($campos);
				if(!$rowspan) $rowspan = 1;
				if(is_array($periodoReferencia)) $rowspan = 2;
			}
		}
		
		/*
		 * GERANDO CABEÇALHO EXCEL, CSV E HTML
		 */ 
		$cc=0;
		$xls = new GeraExcel();
		
		$html = "<script language=\"JavaScript\" src=\"../../includes/funcoes.js\"></script>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
				 <table class='tabela'><tr bgcolor=\"#C0C0C0\"><td align='center'>
						MINISTÉRIO DA EDUCAÇÃO<br>
						Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br>
						<br>
						Expansão da Educação Profissional e Tecnológica
						</td>
					</tr>
				 </table>
				 <table class='tabela'>";
		

		if($_REQUEST['agrupador']) {
			
			$html .= "<tr>";
						
			foreach($_REQUEST['agrupador'] as $col) {
				
				if(!$colspan) $colspan = count($campos);

				$html .= "<td rowspan='".$rowspan."' class='SubTituloCentro'>".$campos[$col]['descricao']."</td>";
				
				$xls->MontaConteudoString(1, $cc, $campos[$col]['descricao']);
				$cc++;
				
			}
			
			if($htmlaux) {
				$html .= $htmlaux; 
			}
		}
		if( is_array($periodoReferencia) ) {
			
			foreach($periodoReferencia as $col) {
				
				$ano = substr($col,4,4)."° Sem. ".substr($col,0,4);
				$html .= "<td colspan=\"5\" class='SubTituloCentro'>".$ano."</td>";
				
				$xls->MontaConteudoString(0, $cc, $ano);
				$cc++;
				$xls->MontaConteudoString(0, $cc, $ano);
				$cc++;
				$xls->MontaConteudoString(0, $cc, $ano);
				$cc++;
				$xls->MontaConteudoString(0, $cc, $ano);
				$cc++;
				$xls->MontaConteudoString(0, $cc, $ano);
				$cc++;
				
			}
			
			$html .= "</tr>";
			$cc = count($_REQUEST['agrupador']);
			$html .= "<tr>";
						
			foreach($periodoReferencia as $col) {
				
				$html .= "<td class='SubTituloCentro'> Turmas </td>";
				$html .= "<td class='SubTituloCentro'> Ofertas </td>";
				$html .= "<td class='SubTituloCentro'> Vagas </td>";
				$html .= "<td class='SubTituloCentro'> Matrículas </td>";
				$html .= "<td class='SubTituloCentro'> Concluintes </td>";
				
				$xls->MontaConteudoString(1, $cc, "Turmas");
				$cc++;
				$xls->MontaConteudoString(1, $cc, "Ofertas");
				$cc++;
				$xls->MontaConteudoString(1, $cc, "Vagas");
				$cc++;
				$xls->MontaConteudoString(1, $cc, "Matrículas");
				$cc++;
				$xls->MontaConteudoString(1, $cc, "Concluintes");
				$cc++;
				
			}
			
			$html .= "</tr>";
			
		}else{
			$html .= "</tr>";
		}
		/*
		 * FIM - GERANDO CABEÇALHO EXCEL, CSV E HTML
		 */ 
		

		$i=2;
		foreach($dadoscsv as $registro) {
			
			$cc=0;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			
			if($_REQUEST['agrupador']) {

				$html .= "<tr bgcolor='".$marcado."'>";
				
				foreach($_REQUEST['agrupador'] as $col) {
					
					$html .= "<td>".$registro[$col]."</td>";
					
					$xls->MontaConteudoString($i, $cc, $registro[$col]);
					$cc++;

				}
				
			}
			
			if( is_array($periodoReferencia) ) {
				
				$refanoprimeiraoferta = referencia( $registro['anoprimoferta'], $registro['semestre'], $referencia );
				$refanoultimaoferta   = referencia( $registro['anoultoferta'], $registro['semestre'], $referencia );
				$total 				  = $registro['qtdsemaula'] + $registro['qtdsemestagio'];
				
				foreach($periodoReferencia as $k => $col) {

					$turmas = T_TUR($k, $refanoprimeiraoferta, $refanoultimaoferta, $total)*$registro['qtdturmas'];
					$cor = $turmas == 0 ? 'red' : 'black';
					$html .= "<td align='center' style='color: ".$cor."'>".$turmas."</td>";
					
					$ofertas = T_OFER($turmas, $k, $refanoprimeiraoferta, $registro['vagas']);
					$cor = $ofertas == 0 ? 'red' : 'black';
					$html .= "<td align='center' style='color: ".$cor."'>".$ofertas."</td>";
					
					$vagas = vagas( $turmas, $registro['vagas'], $registro['cargahr'], $total, $registro['qtdturmas'], $registro['nivid'] );
					$cor = $vagas == 0 ? 'red' : 'black';
					$html .= "<td align='center' style='color: ".$cor."'>".$vagas."</td>";
					
					$metsemestre = substr($col,4,4);
					$metano 	 = substr($col,0,4);
					$mettx 		 = mettx( $registro['intid'], $registro['campid'], $metano, $metsemestre );
					$matriculas  = resultadomatriculas( $vagas, $mettx['mettxocupacao'] );
					$html .= "<td align='center' style='color: ".$cor."'>".$matriculas."</td>";
					
					$concluintes = T_CONC($turmas, $k, $refanoprimeiraoferta, $total) * ofertaturmas($registro['vagas'], $registro['cargahr'], $total, $registro['nivid']) * $mettx['mettxconclusao'];//{Essa informação vem da tabela academico.metas}
					$html .= "<td align='center' style='color: ".$cor."'>".number_format($concluintes,2,",",".")."</td>";
					
					$xls->MontaConteudoString($i, $cc, $turmas);
					$cc++;
					$xls->MontaConteudoString($i, $cc, $ofertas);
					$cc++;
					$xls->MontaConteudoString($i, $cc, $vagas);
					$cc++;
					$xls->MontaConteudoString($i, $cc, $matriculas);
					$cc++;
					$xls->MontaConteudoString($i, $cc, number_format($concluintes,2,",","."));
					$cc++;
					
				}
				
				$html .= "</tr>";
			}else{
				$html .= "</tr>";
			}
			

			$i++;

		}
		
		$html .= "</table>";
		
	} else {
		
		
		$html = "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
				 <table class='tabela'><tr bgcolor=\"#C0C0C0\"><td colspan='19' align='center'>MINISTÉRIO DA EDUCAÇÃO<br>Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br><br>Expansão da Educação Profissional e Tecnológica</td></tr><tr><td style='color:#ff0000; text-align:center'>Não foram encontrados registros.</td></tr></table>";
		
	}
	
	
	
	switch(substr($_POST['exibir'],10,3)) {
		case 'csv':
			header("Content-type: application/octet-stream");
			header("Content-Disposition: attachment; filename=\"my-data.csv\"");
			echo $csv;
			break;
		case 'xls':
			ob_clean();
			$xls->GeraArquivo();
			break;
		case 'htm':
			echo $html;
			break;
			
	}
}

function montaProjecao( ){
	
	global $db;
	
	$sql = "SELECT
				min(ofcanoprimeiraoferta) as inicio,
				max(ofcanoultimaoferta) as fim
			FROM
				academico.ofertacursos
			WHERE 
				ofcanoultimaoferta < 3000";
	$anos = $db->pegaLinha($sql);
	$periodo = ((int)$anos['fim'] - (int)$anos['inicio']+1)*2;
	$anoInicio = $anos['inicio'];
	$periodoReferencia = Array();
	for($i=1;$i<=$periodo;$i++){
		$periodoReferencia[$i] = $anoInicio.'1';
		$i++;
		$periodoReferencia[$i] = $anoInicio.'2';
		$anoInicio++;
	}
	return $periodoReferencia;
}

function montaPeriodoProjecao( $anoInicio, $anoFim, $referencia ){
	
	foreach( $referencia as $k => $ref ){
		if( $ref == $anoInicio.'1' ){
			$periodoReferencia[$k] = $anoInicio.'1';
		}elseif( $ref == $anoInicio.'2' ){
			$periodoReferencia[$k] = $anoInicio.'2';
			if( $anoInicio < $anoFim ){
				$anoInicio++;
			}
		}
	}
	return $periodoReferencia;
}

function referencia( $ano, $semestre, $periodoReferencia ){
	
	$key = array_keys($periodoReferencia,$ano.$semestre);
	if( $key ){
		$key = $key[0];
	}else{
		$key = 0;
	}
	return $key;
}

function T_TUR($anosemprojetado, $anosemprimeiraoferta, $anosemultimaoferta, $total){
//	ver($anosemprojetado,$anosemprimeiraoferta,$anosemultimaoferta,$total);
	if( ($anosemprojetado <> "") and ($anosemprimeiraoferta <> "") and ($anosemultimaoferta <> "") and ($total <> "") ){
		if ($anosemprojetado >= $anosemprimeiraoferta){
			$T_TUR = (int)((($total)+1)/2);
			
			if( ((int)(($anosemprojetado-$anosemprimeiraoferta)/2)+1) < $T_TUR ){
				$T_TUR = ((int)(($anosemprojetado-$anosemprimeiraoferta)/2)+1);
			}
			if ( ((int)(($anosemprojetado+$anosemprimeiraoferta)/2)) <> (($anosemprojetado+$anosemprimeiraoferta)/2) ){
				if( ((int)($total/2)) <> ($total/2) ){
					if($T_TUR = (int)((($total)+1)/2)){
						$T_TUR = $T_TUR - 1;
					}
				}
			}
			if($anosemprojetado>$anosemultimaoferta){
				$T_TUR = $T_TUR - (int)(($anosemprojetado-$anosemultimaoferta)/2);
			}
			if($T_TUR < 0){
				$T_TUR = 0;
			}
		}else{
			$T_TUR = 0;
		}
	}else{
		$T_TUR = 0; 
	}
	return $T_TUR;
}

function ofertaturmas($ofcnumvagas, $ofccargahoraria, $total, $nivid){
	if ($nivid == 4){
		if( ($ofccargahoraria != 0 && $ofccargahoraria != '') && ($total != 0 && $total != '') ){
	 		$ofertaturmas = (($ofcnumvagas*$ofccargahoraria)/400)/$total;
		}
	}else{
		$ofertaturmas = $ofcnumvagas;
	}
	return $ofertaturmas;
}

function T_OFER( $resultadoturmas, $anosemprojetado, $anosemprimeiraoferta, $vagas ){
   $T_OFER = 0;
   if ( (($anosemprojetado % 2) == ($anosemprimeiraoferta % 2)) && ($resultadoturmas > 0) ){
      $T_OFER = $vagas;
   }
   return $T_OFER;
}	

function vagas( $resultadoturmas, $ofcnumvagas, $ofccargahoraria, $total, $ofcnumturmas, $nivid ){
	if ($resultadoturmas > 0){
		$resultadovagas = ($resultadoturmas * ofertaturmas($ofcnumvagas, $ofccargahoraria, $total, $nivid))/$ofcnumturmas;
	}else{
		$resultadovagas = 0;
	}
	return (int)$resultadovagas;
}

function resultadomatriculas( $resultadovagas, $mettxocupacao ){
	if ( $resultadovagas > 0 ){
		$resultadomatriculas = $resultadovagas * $mettxocupacao;//{Essa informação vem da tabela academico.metas}
	}else{
		$resultadomatriculas = 0;
	}
	return number_format($resultadomatriculas,2,",",".");
}

function mettx( $entidinstituto, $entidcampus, $metano, $metsemestre ){
	
	global $db;
	
	$entidcampus = $entidcampus ? "entidcampus = ".$entidcampus : "";
	
	$sql = "SELECT
				mettxocupacao,
				mettxconclusao
			FROM 
				academico.metas
			WHERE
				--entidinstituto = {$entidinstituto}
				{$entidcampus}
				AND metano = {$metano}
				AND metsemestre = {$metsemestre}";
				
	return $db->pegaLinha($sql);
}

function T_CONC($resultadoturmas, $anosemprojetado, $anosemprimeiraoferta, $total){
   $T_CONC = 0;
   $a = 0;
   if( $resultadoturmas>0 ){
      $a = $anosemprojetado - $total + 1;
      if( $a >= $anosemprimeiraoferta ){
        if(  ($a % 2) == ($anosemprimeiraoferta % 2) ){
           $T_CONC = 1;
        }
      }
   }
   return $T_CONC;
}

if(substr($_POST['exibir'],0,9) == "relatorio") {

	unset($vals);
	if($_POST['entid'][0]) {
		foreach($_POST['entid'] as $entid) {
			$vals[] = $entid;
		}
		$filtroSQL[] = "entinst.entid IN(".implode(",",$vals).")";
	}
	unset($vals);
	if($_POST['campus'][0]) {
		foreach($_POST['campus'] as $camp) {
			$vals[] = $camp;
		}
		$filtroSQL[] = "entcampus.entid IN(".implode(",",$vals).")";
	}
	/* Gerar cabeçalho XLS */
	$nomeDoArquivoXls = "my-data";
	
	mostrar( $_REQUEST['anoinicio'], $_REQUEST['anofim'], $filtroSQL );
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "adodb/adodb.inc.php";


echo "<br/>";
monta_titulo( $titulo_modulo, '' );
//$titulo_modulo = "Sistema de Informações Gerenciais";
//monta_titulo( $titulo_modulo,'Relatório Nota Técnica');
?>

<script>
function abrirPopup(){
	window.open('', 'abrirPopup', 'toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=800,height=600');	
}

function abrepopupUnidade(){
	for(i=0;i<document.formulario.orgid.length;i++) {
		if(document.formulario.orgid[i].checked) {
			var itemselecionado = document.formulario.orgid[i].value;
		}
	}
	janela = window.open('/sig/combo_unidades.php?tipoensino='+itemselecionado,'Unidades','width=400,height=400,toolbar=yes,status=yes,scrollbars=1,top=200,left=480');
	janela.focus();
}
function exibirRelatorio() {
	var formulario = document.formulario;
	//Filtros
	
	selectAllOptions( agrupador );
	
	selectAllOptions(document.formulario.entid);
	selectAllOptions(document.formulario.campus);
	
	return true;	
}

</script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<div id="loader-container">
	<div id="loader">
		<img src="../imagens/wait.gif" border="0" align="middle">
		<span>Aguarde! Carregando Dados...</span>
	</div>
</div>
<form method="post" name="formulario" target="abrirPopup">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
	<tr><td colspan="2">Filtros</td></tr>
	<tr id="coluna" >
		<td class="SubTituloDireita" valign="top" width="20%">
			Colunas
		</td>
		<td width="80%">
<?
$agrupadorHtml =
<<<EOF
    <table>
        <tr valign="middle">
            <td>
                <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <!--
                <img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
                -->
                <img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
            </td>
            <td>
                <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
            </td>
        </tr>
    </table>
    <script type="text/javascript" language="javascript">
        limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
        limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
        {POVOAR_ORIGEM}
        {POVOAR_DESTINO}
        sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
    </script>
EOF;

$agrupador = new Agrupador( 'formulario', $agrupadorHtml );
// exibe agrupador
$agrupador->setOrigem( 'naoAgrupador', null, $origem );
$agrupador->setDestino( 'agrupador', null, $destino );
$agrupador->exibir();


?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Instituição</td>
		<td>
		<?
		$sqlComboInstituicao = "SELECT 
									ent.entid as codigo,
									ent.entnome as descricao 
								FROM
									entidade.entidade ent
								INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid
								WHERE 
									funid = 11 --Instituição
									AND ent.entstatus = 'A'";
		$entid = array(0 => array("codigo" => "", "descricao" => "Todas"));
		combo_popup("entid", $sqlComboInstituicao, "Entidade", "400x400", 0, '', "", "S", false, false, 5, 400);
		?>
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita" valign="top">Campus</td>
        <td>
	        <select multiple="multiple" size="5" name="campus[]" id="campus" ondblclick="abrepopupCampus();" class="CampoEstilo" style="width:400px;">
	        	<option value="">Todas</option>
	        </select>
        </td>
   	</tr>
    <tr>
        <td class="SubTituloDireita" valign="top">Periodo</td>
        <td>
        	Ano Inicial:<?=campo_texto( 'anoinicio', '', 'S', '', 5, 4, '', '', 'left', '', 0, 'id="anoinicio"','' ); ?>
        	Ano Final:<?=campo_texto( 'anofim', '', 'S', '', 5, 4, '', '', 'left', '', 0, 'id="anofim"','' ); ?>
		</td>
   	</tr>
   	<tr bgcolor="#C0C0C0">
   		<td>&nbsp;</td>
		<td>
			<input type="submit" value="Gerar HTML" onclick="document.getElementById('exibir').value='relatorio_html';exibirRelatorio();">
			<input type="submit" value="Gerar XLS"  onclick="document.getElementById('exibir').value='relatorio_xls';exibirRelatorio();"> 
			<input type="hidden" id="exibir" name="exibir" value="relatorio">
		</td>
   	</tr>
</table>
</form>
<script>
$('loader-container').hide();

function abrepopupCampus(){
	var itemselecionado = '';
	for(i=0;i<document.formulario.entid.length;i++) {
		if(document.formulario.entid[i]) {
			itemselecionado = itemselecionado+','+document.formulario.entid[i].value;
		}
	}
	itemselecionado = "{}"+itemselecionado+"{}";
	janela = window.open('academico.php?modulo=relatorio/combo_campus&acao=A&institutos='+itemselecionado,'Unidades','width=400,height=400,toolbar=no,status=no,scrollbars=1,top=200,left=480');
	janela.focus();
}
</script>
