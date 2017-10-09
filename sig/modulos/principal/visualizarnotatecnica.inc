<?

ini_set("memory_limit", "1024M");

function salvaPDF($id){
	global $db, $_funcoes, $anosanalisados,$tituloitens,$titulocursos;

	$ntcid = $id;

if($_SESSION['sig_var']['iscampus'] == 'sim') {
	$unidadeid = $db->pegaUm("SELECT fea.entid FROM entidade.entidade ent
							  LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
							  LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid   
							  WHERE ent.entid='".$_SESSION['sig_var']['entid']."'");
	$campusfixo = "AND e.entid ='".$_SESSION['sig_var']['entid']."'";
} elseif($_SESSION['sig_var']['iscampus'] == 'nao') {
	$unidadeid = $_SESSION['sig_var']['entid'];
}

//Chamando a classe de FPDF
require('../../includes/fpdf/fpdf.inc');

$PDF = new FPDF( 'P','cm','A4' ); //documento em formato de retrato, medido em cm e no tamanho A4
$PDF -> SetMargins(1, 1, 1); // margem esquerda = 3 , superior = 3 e direita = 2.
$PDF -> SetAuthor(SIGLA_SISTEMA); //informando o autor do documento.
$PDF -> SetTitle(SIGLA_SISTEMA); //informando o título do documento.
$PDF -> AddPage(); //adicionando um nova página

/*
 * CRIANDO CABEÇALHO DO PDF
 */
$PDF ->Image("../imagens/brasao.JPG",1,0.8,1.2,1.2); //endereco da imagem,posicao X(horizontal),posicao Y(vertical), tamanho altura, tamanho largura
$PDF ->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF->SetX(2.5);
$PDF->Cell(6,0,'MINISTÉRIO DA EDUCAÇÃO',0,'P');
$PDF->SetX(2.5);
$PDF->Cell(6,0.5,'Gabinete do Ministro - Esplanada dos Ministérios - Bloco "L" 8º andar',0,'P');
$PDF->SetX(2.5);
$PDF->Cell(6,1,'CEP: 70047-900 - Brasília - DF - Brasil',0,'P');
$PDF->SetX(2.5);
$PDF->Cell(6,1.5,'Tel. (61) 2104.8520 / 2104.8740 - FAX nº (61) 2104.9198 ',0,'P');
/*
 * FIM
 * CRIANDO CABEÇALHO DO PDF
 */


/*
 * INÍCIO DA NOTA TECNICA
 */
$datacriacao = $db->pegaUm("SELECT to_char(ntcdatageracao,'dd/mm/yyyy HH24:MI:SS') AS data 
							FROM academico.notatecnica 
							WHERE ntcid='".$ntcid."'");
/*
 * VERIFICANDO SE FOI ESCRITO ALGUMA AGENDA PROPOSTA
 */
(!$_REQUEST['ntcagproposta'])? $agendaproposta = 'Não informado.' : $agendaproposta = $_REQUEST['ntcagproposta'];
(!$_REQUEST['ntcpainaug'])? $parecerinauguracao = 'Não informado.' : $parecerinauguracao = $_REQUEST['ntcpainaug'];

 
$desc_unidade = $db->pegaUm("SELECT entnome FROM entidade.entidade WHERE entid='".$unidadeid."'");
$edtdsc = $db->pegaUm("SELECT edtdsc FROM academico.entidadedetalhe WHERE entid='".$unidadeid."'");

(!$edtdsc)? $edtdsc = 'Não informado.' : $edtdsc = $edtdsc;

/*
 * DADOS DE ENDEREÇO DA UNIDADE ORÇAMENTARIA
 */
$dadosUnidade = $db->carregar("
SELECT ende.*
		FROM
		entidade.endereco as ende
	WHERE ende.entid = '".$unidadeid."'");
$dadosendvazio = 1;

if(trim($dadosUnidade[0]['endlog'])) {
	$enddadosUnidade = $dadosUnidade[0]['endlog'];
	$dadosendvazio = 0;
}
if(trim($dadosUnidade[0]['endnum'])) {
	$enddadosUnidade .= ', '.$dadosUnidade[0]['endnum'];
	$dadosendvazio = 0;
}
if(trim($dadosUnidade[0]['endbai'])) {
	$enddadosUnidade .= ', '.$dadosUnidade[0]['endbai'];
	$dadosendvazio = 0;
}
if(trim($dadosUnidade[0]['estuf'])) {
	$enddadosUnidade .= ', '.$dadosUnidade[0]['estuf'];
	$dadosendvazio = 0;
}
if(trim($dadosUnidade[0]['endcep'])) {
	$enddadosUnidade .= ', '.$dadosUnidade[0]['endcep'];
	$dadosendvazio = 0;
}
if($dadosendvazio != 0) {
	$enddadosUnidade = 'Não informado.';
}
/*
 * FIM
 * DADOS DE ENDEREÇO DA UNIDADE ORÇAMENTARIA
 */

/*
 * MUNICIPIOS QUE FAZEM FRONTEIRA COM O MUNICIPIO DA ENTIDADE ORÇAMENTARIA
 */
$sql = "SELECT mun.mundescricao FROM territorios.municipiosvizinhos muv 
				LEFT JOIN territorios.municipio mun ON muv.muncodvizinho = mun.muncod 
				WHERE muv.muncod='".$dadosUnidade[0]['muncod']."'";
$dadosvizinhos = $db->carregar($sql);

if($dadosvizinhos[0]) {
	foreach($dadosvizinhos as $viz) {
		$vizs[]= $viz['mundescricao'];
	}
	$lim = implode(", ", $vizs);
} else {
	$lim = "Não informado";
}
/*
 * FIM
 * MUNICIPIOS QUE FAZEM FRONTEIRA COM O MUNICIPIO DA ENTIDADE ORÇAMENTARIA
 */

/*
 * DADOS DO DIRIGENTE DA UNIDADE ORÇAMENTARIA
 */
$sql = "SELECT ent.* FROM entidade.entidade ent 
		LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid  
		WHERE fea.entid = '".$unidadeid."' AND fen.funid='". $_funcoes[$_SESSION['sig_var']['orgid']]['unidade'] ."'";
$dirigente = $db->carregar($sql);

$dadostelvazio = 1;	
if(trim($dirigente[0]['entnumdddresidencial'])) {
	$teldadosDirigente = "Residencial: (".trim($dirigente[0]['entnumdddresidencial']).") ".trim($dirigente[0]['entnumresidencial']);
	$dadostelvazio = 0;
}
if(trim($dirigente[0]['entnumcomercial'])) {
	(!$dirigente[0]['entnumramalcomercial'])? $ramal = "" : $ramal = " ramal ".trim($dirigente[0]['entnumramalcomercial']);
	$teldadosDirigente .= ", Comercial: (".trim($dirigente[0]['entnumdddcomercial']).") ".trim($dirigente[0]['entnumcomercial']).$ramal;
	$dadostelvazio = 0;
}
if(trim($dirigente[0]['entnumfax'])) {
	(!$dirigente[0]['entnumramalfax'])? $ramal = "" : $ramal = " ramal ".trim($dirigente[0]['entnumramalfax']);
	$teldadosDirigente .= ", FAX: (".trim($dirigente[0]['entnumdddfax']).") ".trim($dirigente[0]['entnumfax']).$ramal;
	$dadostelvazio = 0;
}
if($dadostelvazio != 0) {
	$teldadosDirigente = 'Não informado';
}
/*
 * FIM
 * DADOS DO DIRIGENTE DA UNIDADE ORÇAMENTARIA
 */



// ----------------------- Dados dos Campus  ----------------------- 

$sql = "SELECT DISTINCT
		 cam.cmpid,
		 cam.cmpobs,
    	 cam.cmpsituacaoobra,
		 e.entid as campusid,
   		 e.entnome as campus, 
    	 en.estuf AS uf,
    	 cam.cmpexistencia,
    	 cam.cmpsituacao,
    	 cam.cmpinstalacao,
    	 to_char(cam.cmpdataatualizacao, 'dd/mm/yyyy HH24:MI:SS') as cmpdataatualizacao,
    	 m.muncod AS municipioid,
		 m.mundsc AS municipio
		FROM entidade.entidade e 
		INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
		INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
		INNER JOIN entidade.entidade au ON fea.entid = au.entid 
		INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = au.entid
		INNER JOIN academico.orgaouo teu ON teu.funid = fen2.funid
		INNER JOIN academico.campus cam ON e.entid = cam.entid					
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = cam.usucpf
		INNER JOIN entidade.endereco en ON ( e.entid = en.entid AND en.tpeid = 1)
		INNER JOIN public.municipio m on m.muncod = en.muncod  
		WHERE fea.entid = '". $unidadeid ."' ".$campusfixo;
$campus = $db->carregar($sql);
	
$PDF->SetY(2.5);
$PDF->SetX(1);
$PDF->SetFont('Arial', 'B', 8); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
$PDF->Cell(19,0.5,'Nota Técnica Nº '.$ntcid.' - Data de criação : '.$datacriacao,1,0,'C',1);
$PDF->ln(); //quebra de linha
$PDF ->SetFillColor(204,204,204); //cor de fundo da tabela
$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF->MultiCell(19,0.5,'AGENDA PROPOSTA: '.$agendaproposta,1,'J',0);
$PDF ->SetFillColor(204,204,204); //cor de fundo da tabela
$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF->MultiCell(19,0.5,'PARECER DO MEC SOBRE A PROPOSTA DE INAUGURAÇÃO: '.$parecerinauguracao,1,'J',0);
$PDF->SetFont('Arial', 'B', 7); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF->Cell(19,0.5,$desc_unidade,1,0,'C',1);
$PDF->ln(); //quebra de linha
$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF->MultiCell(19,0.5,'CARACTERIZAÇÃO DA UNIDADE: '.$edtdsc,1,'J',0);
$PDF->SetFont('Arial', 'B', 7); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF->Cell(19,0.5,'Dados da Unidade',1,0,'L',1);
$PDF->ln(); //quebra de linha
$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF->MultiCell(19,0.5,'ENDEREÇO: '.$enddadosUnidade,1,'J',0);
$PDF->MultiCell(19,0.5,'MUNICÍPIOS LIMÍTROFES: '.$lim,1,'J',0);
$PDF->SetFont('Arial', 'B', 7); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF->Cell(19,0.5,'Dados do Dirigente',1,0,'L',1);
$PDF->ln(); //quebra de linha
$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF->MultiCell(19,0.5,'NOME: '.((!$dirigente[0]['entnome'])? $dirigente[0]['entnome'] = "Não informado" : $dirigente[0]['entnome'] = $dirigente[0]['entnome']),1,'J',0);
$PDF->MultiCell(19,0.5,'FUNÇÃO / OCUPAÇÃO: '.((!$dirigente[0]['fundsc'])? $dirigente[0]['fundsc'] = "Não informado" : $dirigente[0]['fundsc'] = $dirigente[0]['fundsc']),1,'J',0);
$PDF->MultiCell(19,0.5,'TELEFONES: '.$teldadosDirigente,1,'J',0);
$PDF->MultiCell(19,0.5,'E-MAIL: '.((!$dirigente[0]['entemail'])? $dirigente[0]['entemail'] = "Não informado" : $dirigente[0]['entemail'] = $dirigente[0]['entemail']),1,'J',0);

// -------------------   Campus    -------------------
if($campus[0]) {
		foreach($campus as $camp) {
			unset($dadosUnidade);
			unset($dadosendvazio);
			unset($enddadosUnidade);
			unset($lim);
			unset($teldadosDirigente);
			unset($dadosvizinhos);
			unset($vizs);
			unset($situacaoCampus);
			$tipolocalidade = definirtipolocalidade($_SESSION['sig_var']['orgid']);
			$PDF->SetFont('Arial', 'B', 7); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF ->SetFillColor(204,204,204); //cor de fundo da tabela
			$PDF->ln(); //quebra de linha
			$PDF->Cell(19,0.5,$camp['campus'],1,0,'C',1);
			$PDF->ln(); //quebra de linha
			$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$edtdsc = $db->pegaUm("SELECT edtdsc FROM academico.entidadedetalhe WHERE entid='".$camp['campusid']."'");
			$PDF->MultiCell(19,0.5,'CARACTERIZAÇÃO DO CAMPUS: '.((!$edtdsc)? $caracCampus = "Não informado" : $caracCampus = $edtdsc),1,'J',0);
			$dadosUnidade = $db->carregar("
	SELECT ende.*
		FROM
		entidade.endereco as ende
	WHERE ende.entid = '".$camp['campusid']."'");
			$csendvazio = 1;	
			if(trim($dadosUnidade[0]['endlog'])) {
				$enddadosUnidade = $dadosUnidade[0]['endlog'];
				$dadosendvazio = 0;
			}
			if(trim($dadosUnidade[0]['endnum'])) {
				$enddadosUnidade .= ', '.$dadosUnidade[0]['endnum'];
				$dadosendvazio = 0;
			}
			if(trim($dadosUnidade[0]['endbai'])) {
				$enddadosUnidade .= ', '.$dadosUnidade[0]['endbai'];
				$dadosendvazio = 0;
			}
			if(trim($dadosUnidade[0]['estuf'])) {
				$enddadosUnidade .= ', '.$dadosUnidade[0]['estuf'];
				$dadosendvazio = 0;
			}
			if(trim($dadosUnidade[0]['endcep'])) {
				$enddadosUnidade .= ', '.$dadosUnidade[0]['endcep'];
				$dadosendvazio = 0;
			}
			if($dadosendvazio != 0) {
				$enddadosUnidade = 'Não informado.';
			}
			$sql = "SELECT mun.mundescricao FROM territorios.municipiosvizinhos muv 
						LEFT JOIN territorios.municipio mun ON muv.muncodvizinho = mun.muncod 
						WHERE muv.muncod='".$dadosUnidade[0]['muncod']."'";
			$dadosvizinhos = $db->carregar($sql);
				if($dadosvizinhos[0]) {
					foreach($dadosvizinhos as $viz) {
						$vizs[]= $viz['mundescricao'];
					}
					$lim = implode(", ", $vizs);
				} else {
					$lim = "Endereço não informado";
				}
			$sql = "SELECT * FROM entidade.entidade ent 
					LEFT JOIN entidade.funcaoentidade fen ON ent.entid = fen.entid 
					LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
					LEFT JOIN entidade.funcao fun ON fen.funid = fun.funid 
					WHERE fea.entid = '".$camp['campusid']."' AND fen.funid='". $_funcoes[$_SESSION['sig_var']['orgid']]['campus'] ."'";
				$dirigente = $db->carregar($sql);
			$dadostelvazio = 1;	
			if(trim($dirigente[0]['entnumdddresidencial'])) {
				$teldadosDirigente = "Residencial: (".trim($dirigente[0]['entnumdddresidencial']).") ".trim($dirigente[0]['entnumresidencial']);
				$dadostelvazio = 0;
			}
			if(trim($dirigente[0]['entnumcomercial'])) {
				(!$dirigente[0]['entnumramalcomercial'])? $ramal = "" : $ramal = " ramal ".trim($dirigente[0]['entnumramalcomercial']);
				$teldadosDirigente .= ", Comercial: (".trim($dirigente[0]['entnumdddcomercial']).") ".trim($dirigente[0]['entnumcomercial']).$ramal;
				$dadostelvazio = 0;
			}
			if(trim($dirigente[0]['entnumfax'])) {
				(!$dirigente[0]['entnumramalfax'])? $ramal = "" : $ramal = " ramal ".trim($dirigente[0]['entnumramalfax']);
				$teldadosDirigente .= ", FAX: (".trim($dirigente[0]['entnumdddfax']).") ".trim($dirigente[0]['entnumfax']).$ramal;
				$dadostelvazio = 0;
			}
			if($dadostelvazio != 0) {
				$teldadosDirigente = 'Não informado';
			}
			$sql = "SELECT DISTINCT
						 cam.cmpid,
						 cam.cmpobs,
				    	 cam.cmpsituacaoobra,
						 e.entid as campusid,
			    		 e.entnome as campus, 
				    	 en.estuf AS uf,
				    	 cam.cmpexistencia,
				    	 cam.cmpsituacao,
				    	 cam.cmpinstalacao,
				    	 to_char(cam.cmpdataatualizacao, 'dd/mm/yyyy HH24:MI:SS') as cmpdataatualizacao,
				    	 m.muncod AS municipioid,
						 m.mundsc AS municipio
						FROM entidade.entidade e 
						INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
						INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
						INNER JOIN entidade.entidade au ON fea.entid = au.entid 
						INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = au.entid
						INNER JOIN academico.orgaouo teu ON teu.funid = fen2.funid
						INNER JOIN academico.campus cam ON e.entid = cam.entid					
						LEFT JOIN seguranca.usuario usu ON usu.usucpf = cam.usucpf
						INNER JOIN entidade.endereco en ON ( e.entid = en.entid AND en.tpeid = 1)
						INNER JOIN public.municipio m on m.muncod = en.muncod  
						WHERE fea.entid = '". $camp['campusid'] ."' ".$campusfixo;
				
				$campus = $db->carregar($sql);
			$PDF->SetFont('Arial', 'B', 7); //informando a fonte, estilo (B = negrito) e tamanho da fonte	
			$PDF->Cell(19,0.5,'Dados do Campus',1,0,'L',1);
			$PDF->ln(); //quebra de linha
			$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->MultiCell(19,0.5,'ENDEREÇO: '.$enddadosUnidade,1,'J',0);
			$PDF->MultiCell(19,0.5,'MUNICÍPIOS LIMÍTROFES: '.$lim,1,'J',0);
			$_cmpexistencia = array("N"=>"Novo","P"=>"Pré-Existente");
			$_cmpsituacao = array("F"=>"Funcionando","N"=>"Não Funcionando");
			$_cmpinstalacao = array("P"=>"Instalações Provisórias","D"=>"Instalações Definitivas");
			(($_cmpexistencia[$camp['cmpexistencia']])? $situacaoCampus = $_cmpexistencia[$camp['cmpexistencia']].", ": $situacaoCampus = "Não informado, ");
			(($_cmpsituacao[$camp['cmpsituacao']])? $situacaoCampus .= $_cmpsituacao[$camp['cmpsituacao']]: $situacaoCampus .= "Não informado");
			if($camp['cmpsituacao']=='F') {
				$situacaoCampus .= " (".$_cmpinstalacao[$camp['cmpinstalacao']].")";
			}
			$PDF->MultiCell(19,0.5,'SITUAÇÃO DO CAMPUS/UNED: '.$situacaoCampus,1,'J',0);
			$PDF->SetFont('Arial', 'B', 7); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->Cell(19,0.5,'Dados do Dirigente',1,0,'L',1);
			$PDF->ln(); //quebra de linha
			$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->MultiCell(19,0.5,'NOME: '.((!$dirigente[0]['entnome'])? $dirigente[0]['entnome'] = "Não informado" : $dirigente[0]['entnome'] = $dirigente[0]['entnome']),1,'J',0);
			$PDF->MultiCell(19,0.5,'FUNÇÃO / OCUPAÇÃO: '.((!$dirigente[0]['fundsc'])? $dirigente[0]['fundsc'] = "Não informado" : $dirigente[0]['fundsc'] = $dirigente[0]['fundsc']),1,'J',0);
			$PDF->MultiCell(19,0.5,'TELEFONES: '.$teldadosDirigente,1,'J',0);
			$PDF->MultiCell(19,0.5,'E-MAIL: '.((!$dirigente[0]['entemail'])? $dirigente[0]['entemail'] = "Não informado" : $dirigente[0]['entemail'] = $dirigente[0]['entemail']),1,'J',0);
			/*$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
			$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->Cell(5,0.5,'Nome:',1,0,'R',1);
			$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->SetX(6);
			$PDF->Cell(14,0.5,((!$dirigente[0]['entnome'])? $dirigente[0]['entnome'] = "Não informado" : $dirigente[0]['entnome'] = $dirigente[0]['entnome']),1,1,'L',0);
			$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->Cell(5,0.5,'Função / Ocupação:',1,0,'R',1);
			$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->SetX(6);
			$PDF->Cell(14,0.5,((!$dirigente[0]['fundsc'])? $dirigente[0]['fundsc'] = "Não informado" : $dirigente[0]['fundsc'] = $dirigente[0]['fundsc']),1,1,'L',0);
			$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->Cell(5,0.5,'Telefones:',1,0,'R',1);
			$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->SetX(6);
			$PDF->Cell(14,0.5,$teldadosDirigente,1,1,'L',0);
			$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->Cell(5,0.5,'E-mail:',1,0,'R',1);
			$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->SetX(6);
			$PDF->Cell(14,0.5,((!$dirigente[0]['entemail'])? $dirigente[0]['entemail'] = "Não informado" : $dirigente[0]['entemail'] = $dirigente[0]['entemail']),1,1,'L',0);
			*/$PDF ->SetFillColor(204,204,204); //cor de fundo da tabela
			$PDF->SetFont('Arial', 'B', 7); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->Cell(19,0.5,'Informações Acadêmicas e Financeiras',1,0,'L',1);
			$PDF->ln(); //quebra de linha
			$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
			$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->Cell(19,0.5,'Processo seletivo de estudantes',1,0,'L',1);
			$PDF->ln(); //quebra de linha
			$sql = "SELECT 'de '||(to_char(prsinscricaoini,'DD/MM/YYYY') ||' até '|| to_char(prsinscricaofim,'DD/MM/YYYY')) as perinsc, 'de '||(to_char(prsprovaini,'DD/MM/YYYY') ||' até '|| to_char(prsprovafim,'DD/MM/YYYY')) as perprova, to_char(prsinicioaula,'DD/MM/YYYY') AS prsinicioaula FROM academico.campus cam
						  INNER JOIN academico.processoseletivo prs ON prs.cmpid = cam.cmpid  
						  WHERE cam.entid='".$camp['campusid']."'";
			$procEstudantes = $db->carregar($sql);
			if($procEstudantes[0]){
				$PDF->Cell(6,0.5,'Inscrição',1,0,'C',1);
				$PDF->SetX(7);
				$PDF->Cell(6,0.5,'Provas',1,0,'C',1);
				$PDF->SetX(13);
				$PDF->Cell(7,0.5,'Início das aulas',1,0,'C',1);
				$PDF->ln(); //quebra de linha
				$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
				$cor_linha = 1;
				foreach($procEstudantes as $procEst) {
					($cor_linha % 2)? $cor_linha2 = '255,255,255' : $cor_linha2 = '245,245,235';
					$PDF ->SetFillColor($cor_linha2); //cor de fundo da tabela
					$PDF->Cell(6,0.5,$procEst['perinsc'],1,0,'C',1);
					$PDF->SetX(7);
					$PDF->Cell(6,0.5,$procEst['perprova'],1,0,'C',1);
					$PDF->SetX(13);
					$PDF->Cell(7,0.5,$procEst['prsinicioaula'],1,0,'C',1);
					$PDF->ln(); //quebra de linha
					$cor_linha ++;
				}
				
			}
			else{
				$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
				$PDF->Cell(19,0.5,'Não existe(m) Processo(s) Seletivo(s) cadastrado(s).',1,0,'C',0);
				$PDF->ln(); //quebra de linha
			}
			// ---------------- Início do REUNI ----------------
			$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
			$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->Cell(19,0.5,(($tituloitens[$_SESSION['sig_var']['orgid']])?$tituloitens[$_SESSION['sig_var']['orgid']]:$tituloitens['default']),1,0,'L',1);
			$PDF->ln(); //quebra de linha
			unset($n);
			unset($pxColuna);
			unset($n_colunas);
			// Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso não, utilizar o padrão
			if($anosanalisados[$_SESSION['sig_var']['orgid']]) {
				$anos = $anosanalisados[$_SESSION['sig_var']['orgid']];
			} else {
				$anos = $anosanalisados['default'];
			}
			$n = count($anos);
			$n_colunas = 19 / ($n +1);
			$PDF->Cell($n_colunas,0.5,'Itens',1,0,'C',1);
			foreach($anos as $ano) {
				$pxColuna += $n_colunas;
				$PDF->SetX($pxColuna + 1);
				$PDF->Cell($n_colunas,0.5,$ano,1,0,'C',1);
			}
			$paramselects = "(CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $camp['cmpid'] ."') is null 
							THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $camp['cmpid'] ."') END) AS ano_".$ano;
			$sql = "SELECT itm.itmdsc, itm.itmid,
				". $paramselects .",
				tpi.tpimascara 
				FROM academico.item itm 
				LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
				LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
				WHERE tei.orgid = '". $_SESSION['sig_var']['orgid'] ."' AND itm.itmglobal = false
				ORDER BY tei.teiordem";
			$itensREUNI = $db->carregar($sql);
			$PDF->ln();
			$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$cor_linha = 1;
			foreach($itensREUNI AS $itRE){
				($cor_linha % 2)? $cor_linha2 = '255,255,255' : $cor_linha2 = '245,245,235';
				$PDF ->SetFillColor($cor_linha2); //cor de fundo da tabela
				$PDF->Cell($n_colunas,0.5,$itRE['itmdsc'],1,0,'C',1);
				$n_colunas = 19 / ($n +1);
				unset($pxColuna);
				foreach($anos as $ano) {
					$paramselects = "(CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $camp['cmpid'] ."') is null 
								THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $camp['cmpid'] ."') END) AS ano_".$ano;
					$sql = "SELECT itm.itmdsc,
					". $paramselects .",
					tpi.tpimascara 
					FROM academico.item itm 
					LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
					LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
					WHERE tei.orgid = '". $_SESSION['sig_var']['orgid'] ."' AND itm.itmglobal = false
					AND itm.itmid = {$itRE['itmid']}
					ORDER BY tei.teiordem";
					$anoREUNI = $db->carregar($sql);
					$pxColuna += $n_colunas;
					$PDF->SetX($pxColuna + 1);
					$PDF->Cell($n_colunas,0.5,$anoREUNI[0]['ano_'.$ano],1,0,'C',1);
				}
				$cor_linha++;
				$PDF->ln();
			}
			// ---------------- Fim do REUNI ----------------
			// ---------------- Início da Situação Atual ---------------- 
			//$PDF->ln(); //quebra de linha
			$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
			$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->Cell(19,0.5,'Situação Atual',1,0,'L',1);
			$PDF->ln(); //quebra de linha
			$paramselct = "(CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $camp['cmpid'] ."') is null 
					THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $camp['cmpid'] ."') END)  AS ano";
	
			$sql = "SELECT itm.itmdsc,
				   ". $paramselct .",
				   tpi.tpimascara
					FROM academico.item itm 
					LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
					LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
					WHERE tei.orgid = '". $_SESSION['sig_var']['orgid'] ."' AND itm.itmglobal = true
					ORDER BY tei.teiordem";
			$situacaoAtual = $db->carregar($sql);
		if($situacaoAtual[0]){
				$PDF->Cell(9.5,0.5,'Itens',1,0,'C',1);
				$PDF->SetX(10.5);
				$PDF->Cell(9.5,0.5,'Atual',1,0,'C',1);
				$PDF->ln(); //quebra de linha
				$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
				$cor_linha = 1;
				foreach($situacaoAtual as $sitAtu) {
					($cor_linha % 2)? $cor_linha2 = '255,255,255' : $cor_linha2 = '245,245,235';
					$PDF ->SetFillColor($cor_linha2); //cor de fundo da tabela
					$PDF->Cell(9.5,0.5,$sitAtu['itmdsc'],1,0,'C',1);
					$PDF->SetX(10.5);
					$PDF->Cell(9.5,0.5,$sitAtu['ano'],1,0,'C',1);
					$PDF->ln(); //quebra de linha
					$cor_linha ++;
				}
				
			}
			else{
				$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
				$PDF->Cell(19,0.5,'Não existe(m) Iten(s) cadastrado(s).',1,0,'C',0);
				$PDF->ln(); //quebra de linha
			}
			// ---------------- Fim da Situação Atual ----------------
			// ---------------- Inicio Tipo Cursos ----------------
			$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
			$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->Cell(19,0.5,(($titulocursos[$_SESSION['sig_var']['orgid']])?$titulocursos[$_SESSION['sig_var']['orgid']]:$titulocursos['default']),1,0,'L',1);
			$PDF->ln(); //quebra de linha
			unset($n);
			unset($pxColuna);
			unset($n_colunas);
			// Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso não, utilizar o padrão
			if($anosanalisados[$_SESSION['sig_var']['orgid']]) {
				$anos = $anosanalisados[$_SESSION['sig_var']['orgid']];
			} else {
				$anos = $anosanalisados['default'];
			}
			unset($n);
			$n = count($anos);
			$n_colunas = 9.5 / $n;
			$PDF->Cell(3,0.5,'Tipo de Curso',1,0,'C',1);
			$PDF->SetX(4);
			$PDF->Cell(6.5,0.5,'Curso',1,0,'C',1);
			$PDF->SetX(10.5);
			foreach($anos as $ano) {
				$pxColuna += $n_colunas;
				$PDF->Cell($n_colunas,0.5,$ano,1,0,'C',1);
				$PDF->SetX($pxColuna + 10.5);
				$inputs[] = "(SELECT coalesce(cast(cpcqtd as varchar),'') FROM academico.campuscurso cmc1 WHERE cmc1.curid = cmc2.curid AND cmc1.cpcano='".$ano."' AND cmc1.cmpid='".$camp['cmpid']."') as ano".$ano;
			}
			//Divião em vagas modificadas e não modificadas
			$sql = "SELECT cur.curid,cur.curdsc FROM academico.campuscurso cmc2 
			LEFT JOIN public.curso cur ON cur.curid = cmc2.curid 
			LEFT JOIN public.tipocurso tpc ON tpc.tpcid = cur.tpcid 
			WHERE cmc2.cmpid='".$camp['cmpid']."' 
			GROUP BY cmc2.curid, cur.curdsc, tpc.tpcdsc, cur.curid order by cur.curdsc ";
			$tipoCurso = $db->carregar($sql);
			if($tipoCurso[0]){
				foreach($tipoCurso AS $tpCur){
					unset ($vagas);
					foreach($anos as $ano) {
						$sql = "SELECT cmc2.cpcqtd FROM academico.campuscurso cmc2 
								LEFT JOIN public.curso cur ON cur.curid = cmc2.curid 
								LEFT JOIN public.tipocurso tpc ON tpc.tpcid = cur.tpcid 
								WHERE cmc2.cmpid='".$camp['cmpid']."' AND cmc2.cpcano = '$ano'
								AND cur.curid = {$tpCur['curid']}";
						$vagaCurso = $db->carregar($sql);
						$vagas[] = $vagaCurso[0]['cpcqtd']; 
					}
					$total1 = array_sum($vagas);
					$total2 = count($vagas) * $vagas[0];
					if($total1 == $total2){
						$tipoCurso_iguais[] = $tpCur['curid'];
					}
					else{
						$tipoCurso_diferentes[] = $tpCur['curid'];
					}
				}
			}
			
			$sql = "SELECT tpc.tpcdsc, cur.curid, cur.curdsc as curso FROM academico.campuscurso cmc2 
			LEFT JOIN public.curso cur ON cur.curid = cmc2.curid 
			LEFT JOIN public.tipocurso tpc ON tpc.tpcid = cur.tpcid 
			WHERE cmc2.cmpid='".$camp['cmpid']."' 
			GROUP BY cmc2.curid, cur.curdsc, tpc.tpcdsc, cur.curid order by cur.curdsc ";
			
			$tipoCurso = $db->carregar($sql);
			$PDF->ln();
			if($tipoCurso[0] && $tipoCurso_diferentes[0]){
				$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
				$PDF->MultiCell(19,0.5,'Cursos com alterações na quantidade de vagas no decorrer dos anos',1,'J',0);
				$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
				$cor_linha = 1;
				foreach($tipoCurso AS $tpCur){
					if(in_array($tpCur['curid'],$tipoCurso_diferentes)){
						($cor_linha % 2)? $cor_linha2 = '255,255,255' : $cor_linha2 = '245,245,235';
						$PDF ->SetFillColor($cor_linha2); //cor de fundo da tabela
						$PDF->Cell(3,0.5,$tpCur['tpcdsc'],1,0,'C',1);
						$PDF->SetX(4);
						$PDF->Cell(6.5,0.5,$tpCur['curso'],1,0,'L',1);
						$PDF->SetX(10.5);
						$n_colunas = 9.5 / ($n);
						unset($pxColuna);
						foreach($anos as $ano) {
							$sql = "SELECT cmc2.cpcqtd FROM academico.campuscurso cmc2 
								LEFT JOIN public.curso cur ON cur.curid = cmc2.curid 
								LEFT JOIN public.tipocurso tpc ON tpc.tpcid = cur.tpcid 
								WHERE cmc2.cmpid='".$camp['cmpid']."' AND cmc2.cpcano = '$ano'
								AND cur.curid = {$tpCur['curid']}";
							$vagaCurso = $db->carregar($sql);
								$pxColuna += $n_colunas;
								$PDF->Cell($n_colunas,0.5,$vagaCurso[0]['cpcqtd'],1,0,'C',1);
								$PDF->SetX($pxColuna + 10.5);
						}
						$cor_linha++;
						$PDF->ln();
					}
				}
			}
			if($tipoCurso[0] && $tipoCurso_iguais[0]){
				$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
				$PDF->MultiCell(19,0.5,'Cursos sem alterações na quantidade de vagas no decorrer dos anos',1,'J',0);
				$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
				$cor_linha = 1;
				foreach($tipoCurso AS $tpCur){
					if(in_array($tpCur['curid'],$tipoCurso_iguais)){
						($cor_linha % 2)? $cor_linha2 = '255,255,255' : $cor_linha2 = '245,245,235';
						$PDF ->SetFillColor($cor_linha2); //cor de fundo da tabela
						$PDF->Cell(3,0.5,$tpCur['tpcdsc'],1,0,'C',1);
						$PDF->SetX(4);
						$PDF->Cell(6.5,0.5,$tpCur['curso'],1,0,'L',1);
						$PDF->SetX(10.5);
						$n_colunas = 9.5 / ($n);
						unset($pxColuna);
						foreach($anos as $ano) {
							$sql = "SELECT cmc2.cpcqtd FROM academico.campuscurso cmc2 
								LEFT JOIN public.curso cur ON cur.curid = cmc2.curid 
								LEFT JOIN public.tipocurso tpc ON tpc.tpcid = cur.tpcid 
								WHERE cmc2.cmpid='".$camp['cmpid']."' AND cmc2.cpcano = '$ano'
								AND cur.curid = {$tpCur['curid']}";
							$vagaCurso = $db->carregar($sql);
								$pxColuna += $n_colunas;
								$PDF->Cell($n_colunas,0.5,$vagaCurso[0]['cpcqtd'],1,0,'C',1);
								$PDF->SetX($pxColuna + 10.5);
						}
						$cor_linha++;
						$PDF->ln();
					}
				}
			}
			if(!$tipoCurso[0]){
				$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
				$PDF->Cell(19,0.5,'Não existe(m) Curso(s) cadastrado(s).',1,0,'C',0);
				$PDF->ln(); //quebra de linha
			}
		// ---------------- Fim Tipo Cursos ----------------
		// ---------------- Início das Obras ----------------
		$_cmpsituacaoobra = array("L"=>"Licitação de Obras","A"=>"Obras em Andamento","C"=>"Obras Concluídas");
		$PDF ->SetFillColor(204,204,204); //cor de fundo da tabela
		$PDF->SetFont('Arial', 'B', 7); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$PDF->Cell(19,0.5,'Informações sobre Obras',1,0,'L',1);
		$PDF->ln(); //quebra de linha
		$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
		$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$PDF->MultiCell(19,0.5,'SITUAÇÃO SOBRE OBRAS NO CAMPUS/UNED: '.(($_cmpsituacaoobra[$camp['cmpsituacaoobra']])?$_cmpsituacaoobra[$camp['cmpsituacaoobra']]:"Não informado"),1,'J',0);
		/*$PDF->Cell(5,0.5,'Situação sobre obras no campus/uned:',1,0,'R',1);
		$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$PDF->SetX(6);
		$PDF->Cell(14,0.5,(($_cmpsituacaoobra[$camp['cmpsituacaoobra']])?$_cmpsituacaoobra[$camp['cmpsituacaoobra']]:"Não informado"),1,1,'L',0);
		*/// ---------------- Início Obra(s) a ser(em) inaugurada(s) ---------------- 
			//$PDF->ln(); //quebra de linha
			$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
			$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->Cell(19,0.5,'Obra(s) a ser(em) inaugurada(s)',1,0,'L',1);
			$PDF->ln(); //quebra de linha
			$sql = "SELECT obr.obrid, obr.obrdesc, obr.obrdtinicio, obr.obrdttermino, mun.mundescricao||'/'||en.estuf as local, obr.obrcomposicao, sto.stodesc FROM academico.campus cam 
				LEFT JOIN obras.obrainfraestrutura obr ON obr.entidcampus = cam.entid  
				LEFT JOIN entidade.endereco en ON en.endid = obr.endid 
				LEFT JOIN territorios.municipio mun ON mun.muncod = en.muncod 
				LEFT JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
				LEFT JOIN academico.obrainauguradacampus obi ON obr.obrid = obi.obrid 
				WHERE cam.cmpid='".$camp['cmpid']."' AND obr.obsstatus='A' AND obi.obcid IS NOT NULL AND (obr.obrstatusinauguracao ='N' OR obr.obrstatusinauguracao IS NULL) ORDER BY sto.stodesc";
		$obraaserinaugurada = $db->carregar($sql);
		if($obraaserinaugurada[0]){
				foreach($obraaserinaugurada as $obr) {
					$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
					$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->MultiCell(19,0.5,'NOME DA OBRA: '.$obr['obrdesc'],1,'J',0);
					$PDF->MultiCell(19,0.5,'MUNICÍPIO / UF: '.$obr['local'],1,'J',0);
					$PDF->MultiCell(19,0.5,'INÍCIO PROGRAMADO: '.formata_data($obr['obrdtinicio']).' / TÉRMINO PROGRAMADO: '.formata_data($obr['obrdttermino']),1,'J',0);
					$PDF->MultiCell(19,0.5,'DESCRIÇÃO: '.(($obr['obrcomposicao'])?nl2br($obr['obrcomposicao']):"Nenhuma descrição inserida."),1,'J',0);
					/*
					$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->Cell(5,0.5,'Nome da Obra:',1,0,'R',1);
					$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->SetX(6);
					$PDF->Cell(14,0.5,$obr['obrdesc'],1,1,'L',0);
					$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->Cell(5,0.5,'Município/UF:',1,0,'R',1);
					$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->SetX(6);
					$PDF->Cell(14,0.5,$obr['local'],1,1,'L',0);
					$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->Cell(5,0.5,'Descrição:',1,0,'R',1);
					$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->SetX(6);
					$PDF->Cell(14,0.5,(($obr['obrcomposicao'])?nl2br($obr['obrcomposicao']):"Nenhuma descrição inserida."),1,1,'L',0);
					*/
					// --------------------- Fotos --------------------- 
					$sql = "SELECT arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao, to_char(oar.aqodtinclusao,'dd/mm/yyyy') as aqodtinclusao FROM public.arquivo arq
						INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
						WHERE oar.obrid='". $obr['obrid'] ."' AND
		  					aqostatus = 'A' AND
		  				   (arqtipo = 'image/jpeg' OR 
		   		 			arqtipo = 'image/gif' OR 
		   		 			arqtipo = 'image/png') 
						ORDER BY arq.arqid DESC LIMIT 5";
					$fotos = $db->carregar($sql);
					$pos_fotos = $PDF->GetY();
					if($fotos[0] && $pos_fotos > 22){
						$PDF->AddPage();
						$pos_fotos = $PDF->GetY();
					}
					$PDF->Cell(19,0.5,'Fotos',1,1,'L',1);
					
				if($fotos[0]){
						$total_fotos = count($fotos);
						//$PDF->Cell(19,0.5,'Existem '.$total_fotos.' fotos cadastradas.',1,0,'C',0);
						$PDF->ln(0.5); //quebra de linha
						$PDF->ln(0.5); //quebra de linha
						$y = $PDF->GetY();
						$coluna = 0.5;
						$largura_max = 3.25;
						$altura_max = 3.0;
						$linha = 1;
						foreach($fotos AS $f){
							
							$caminho = APPRAIZ . 'arquivos/obras/'. floor((int)$f['arqid']/1000) .'/'. $f['arqid'];
							//$caminho = APPRAIZ . 'arquivos/obras/16/001';

							if(file_exists($caminho)){
								$novo_arquivo = APPRAIZ . 'arquivos/obras/'. floor((int)$f['arqid']/1000) .'/'.$f['arqid'].'_01.'.$f['arqextensao'];
								//$novo_arquivo = APPRAIZ . 'arquivos/obras/16/001_01.jpg';
								copy($caminho,$novo_arquivo);
								
							
								if($coluna <= 19 ){
									(($largura/100) > $largura_max)? $larg = $largura_max : $larg = ($largura/100);
									$coluna += $larg + 0.5;
								}
								if($coluna > 19 ){
									$coluna = 1;
									$y = $y + 3.5;
									$linha++;
								}
								if($y > 22){
									$PDF->AddPage();
									$y = $PDF->GetY();
									$linha = 1;
								}
								list($largura,$altura) = getimagesize($novo_arquivo);
								(($largura/100) > $largura_max)? $larg = $largura_max : $larg = ($largura/100);
								(($altura/100) > $altura_max)? $alt = $altura_max : $alt = ($altura/100);
								$PDF->Image($novo_arquivo,$coluna,$y,$larg,$alt);
								unlink($novo_arquivo);
							
							}
						}
						$PDF->ln($linha * ($largura_max + 0.5));
					}
					else{
						$PDF->Cell(19,0.5,'Não existe(m) foto(s) cadastrada(s).',1,0,'C',0);
					}
					// --------------------- Fotos --------------------- 
					$PDF->ln(0.5); //quebra de linha
				}
				
			}
			else{
				$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
				$PDF->Cell(19,0.5,'Não existem obra(s) a serem inauguradas no campus/uned.',1,0,'C',0);
				$PDF->ln(); //quebra de linha
			}
			// ---------------- Fim Obra(s) a ser(em) inaugurada(s) ----------------
			// ---------------- Início Outra(s) obra(s) do campus/uned ---------------- 
			//$PDF->ln(); //quebra de linha
			$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
			$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
			$PDF->ln(); //quebra de linha
			$PDF->Cell(19,0.5,'Outra(s) obra(s) do campus/uned',1,0,'L',1);
			$PDF->ln(); //quebra de linha
			$sql = "SELECT obr.obrid, obr.obrdesc, obr.obrdtinicio, obr.obrdttermino, mun.mundescricao||'/'||en.estuf as local, obr.obrcomposicao, sto.stodesc FROM academico.campus cam 
				LEFT JOIN obras.obrainfraestrutura obr ON obr.entidcampus = cam.entid  
				LEFT JOIN entidade.endereco en ON en.endid = obr.endid 
				LEFT JOIN territorios.municipio mun ON mun.muncod = en.muncod 
				LEFT JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
				LEFT JOIN academico.obrainauguradacampus obi ON obr.obrid = obi.obrid 
				WHERE cam.cmpid='".$camp['cmpid']."' AND obr.obsstatus='A' AND obi.obcid IS NULL AND (obr.obrstatusinauguracao ='N' OR obr.obrstatusinauguracao IS NULL) ORDER BY sto.stodesc";
			$obraaserinaugurada = $db->carregar($sql);
			if($obraaserinaugurada[0]){
				foreach($obraaserinaugurada as $obr) {
					$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
					$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->MultiCell(19,0.5,'NOME DA OBRA: '.$obr['obrdesc'],1,'J',0);
					$PDF->MultiCell(19,0.5,'MUNICÍPIO / UF: '.$obr['local'],1,'J',0);
					$PDF->MultiCell(19,0.5,'INÍCIO PROGRAMADO: '.formata_data($obr['obrdtinicio']).' / TÉRMINO PROGRAMADO: '.formata_data($obr['obrdttermino']),1,'J',0);
					$PDF->MultiCell(19,0.5,'DESCRIÇÃO: '.(($obr['obrcomposicao'])?nl2br($obr['obrcomposicao']):"Nenhuma descrição inserida."),1,'J',0);
					/*
					$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->Cell(5,0.5,'Nome da Obra:',1,0,'R',1);
					$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->SetX(6);
					$PDF->Cell(14,0.5,$obr['obrdesc'],1,1,'L',0);
					$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->Cell(5,0.5,'Município/UF:',1,0,'R',1);
					$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->SetX(6);
					$PDF->Cell(14,0.5,$obr['local'],1,1,'L',0);
					$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->Cell(5,0.5,'Descrição:',1,0,'R',1);
					$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
					$PDF->SetX(6);
					$PDF->Cell(14,0.5,(($obr['obrcomposicao'])?nl2br($obr['obrcomposicao']):"Nenhuma descrição inserida."),1,1,'L',0);
					*/// --------------------- Fotos --------------------- 
					$sql = "SELECT arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao, to_char(oar.aqodtinclusao,'dd/mm/yyyy') as aqodtinclusao FROM public.arquivo arq
						INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
						WHERE oar.obrid='". $obr['obrid'] ."' AND
		  					aqostatus = 'A' AND
		  				   (arqtipo = 'image/jpeg' OR 
		   		 			arqtipo = 'image/gif' OR 
		   		 			arqtipo = 'image/png') 
						ORDER BY arq.arqid DESC LIMIT 5";
					$fotos = $db->carregar($sql);
					$pos_fotos = $PDF->GetY();
					if($fotos[0] && $pos_fotos > 22){
						$PDF->AddPage();
						$pos_fotos = $PDF->GetY();
					}
					$PDF->Cell(19,0.5,'Fotos',1,1,'L',1);
					
					if($fotos[0]){
						$total_fotos = count($fotos);
						//$PDF->Cell(19,0.5,'Existem '.$total_fotos.' fotos cadastradas.',1,0,'C',0);
						$PDF->ln(0.5); //quebra de linha
						$PDF->ln(0.5); //quebra de linha
						$y = $PDF->GetY();
						$coluna = 0.5;
						$largura_max = 3.25;
						$altura_max = 3.0;
						$linha = 1;
						foreach($fotos AS $f){
							$caminho = APPRAIZ . 'arquivos/obras/'. floor((int)$f['arqid']/1000) .'/'. $f['arqid'];
							//$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor((int)$f['arqid']/1000) .'/'. $f['arqid'];

							if(file_exists($caminho)){
								$novo_arquivo = APPRAIZ . 'arquivos/obras/'. floor((int)$f['arqid']/1000) .'/'.$f['arqid'].'_01.'.$f['arqextensao'];
								//$novo_arquivo = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor((int)$f['arqid']/1000) .'/'.$f['arqid'].'_01.'.$f['arqextensao'];
								copy($caminho,$novo_arquivo);
							
								if($coluna <= 19 ){
									(($largura/100) > $largura_max)? $larg = $largura_max : $larg = ($largura/100);
									$coluna += $larg + 0.5;
								}
								if($coluna > 19 ){
									$coluna = 1;
									$y = $y + 3.5;
									$linha++;
								}
								if($y > 22){
									$PDF->AddPage();
									$y = $PDF->GetY();
									$linha = 1;
								}
								list($largura,$altura) = getimagesize($novo_arquivo);
								(($largura/100) > $largura_max)? $larg = $largura_max : $larg = ($largura/100);
								(($altura/100) > $altura_max)? $alt = $altura_max : $alt = ($altura/100);
								$PDF->Image($novo_arquivo,$coluna,$y,$larg,$alt);
								unlink($novo_arquivo);
							
							}
						}
						$PDF->ln($linha * ($largura_max + 0.5));
					}
					else{
						$PDF->Cell(19,0.5,'Não existe(m) foto(s) cadastrada(s).',1,0,'C',0);
					}
					// --------------------- Fotos --------------------- 
					$PDF->ln(0.5); //quebra de linha
				}
				
			}
			else{
				$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
				$PDF->Cell(19,0.5,'Não existem Outra(s) obra(s) do campus/uned.',1,0,'C',0);
				$PDF->ln(); //quebra de linha
			}
			// ---------------- Fim Outra(s) obra(s) do campus/uned ----------------
		
		$PDF->MultiCell(19,0.5,'INFORMAÇÕES ADICIONAIS: '.(($camp['cmpobs'])?$camp['cmpobs']:"Não informado."),1,'J',0);
		// ---------------- Fim das Obras ----------------
		}
}
// ---------------- Início Sobre a Nota ----------------
		// Carregando os dados sobre a nota técnica, diferencia unidade / campus
		if($_SESSION['sig_var']['iscampus'] == 'sim') {
			$sql = "SELECT to_char(cam.cmpdataatualizacao, 'dd/mm/yyyy HH24:MI:SS') as cmpdataatualizacao, 
							usu.usunome, 
							usu.usufonenum, 
							usu.usufoneddd, 
							usu.usuemail 
					FROM academico.campus cam 
					LEFT JOIN seguranca.usuario usu ON usu.usucpf = cam.usucpf
					WHERE cam.entid = '".$_SESSION['sig_var']['entid']."'";
			
		} elseif($_SESSION['sig_var']['iscampus'] == 'nao') {
			$sql = "SELECT to_char(cam.cmpdataatualizacao, 'dd/mm/yyyy HH24:MI:SS') as cmpdataatualizacao, 
							usu.usunome, 
							usu.usufonenum, 
							usu.usufoneddd, 
							usu.usuemail 
					FROM academico.campus cam 
					LEFT JOIN seguranca.usuario usu ON usu.usucpf = cam.usucpf 
					LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid 
					LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
					LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
					LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid 
					WHERE uor.entid = '".$unidadeid."' ORDER BY cmpdataatualizacao DESC LIMIT 1";
		}
		$sobrenotatec = $db->pegaLinha($sql);
		$PDF->ln(); //quebra de linha
		$PDF ->SetFillColor(204,204,204); //cor de fundo da tabela
		$PDF->SetFont('Arial', 'B', 7); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$PDF->Cell(19,0.5,'Sobre a Nota Técnica',1,0,'L',1);
		$PDF->ln(); //quebra de linha
		$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
		$PDF ->SetFillColor(234,234,234); //cor de fundo da tabela
		$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$PDF->MultiCell(19,0.5,'DATA DA ÚLTIMA ATUALIZAÇÃO: '.(($sobrenotatec['cmpdataatualizacao'])?$sobrenotatec['cmpdataatualizacao']:"Não atualizado"),1,'J',0);
		$PDF->MultiCell(19,0.5,'TÉCNICO RESPONSÁVEL: '.(($sobrenotatec['usunome'])?$sobrenotatec['usunome']:"Não atualizado"),1,'J',0);
		if($sobrenotatec['usufonenum']) { 
				$telNT =  "(".$sobrenotatec['usufoneddd'].") ".$sobrenotatec['usufonenum'];
			} else {
				$telNT =  "Não atualizado";
		}
		$PDF->MultiCell(19,0.5,'TELEFONE: '.$telNT,1,'J',0);
		$PDF->MultiCell(19,0.5,'E-MAIL: '.(($sobrenotatec['usuemail'])?$sobrenotatec['usuemail']:"Não atualizado"),1,'J',0);			
		// ---------------- Inicio Em caso de Dùvida ----------------
		$PDF->ln(); //quebra de linha
		$PDF ->SetFillColor(204,204,204); //cor de fundo da tabela
		$PDF->SetFont('Arial', 'B', 7); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$PDF->Cell(19,0.5,'EM CASO DE DÚVIDAS CONTATAR:',0,0,'L',0);
		$PDF->ln(); //quebra de linha
		$PDF->SetFont('Arial', 'B', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
		$PDF->Cell(19,0.5,'João Paulo Bachur',0,0,'L',0);
		$PDF->ln(); //quebra de linha
		$PDF->Cell(19,0.5,'Chefe de Gabinete do Ministro de Estado da Educação',0,0,'L',0);
		$PDF->ln(); //quebra de linha
		$PDF->Cell(19,0.5,'Telefones: (61) 2104-8740',0,0,'L',0);
		$PDF->ln(); //quebra de linha
		$PDF->Cell(19,0.5,'E-mail: joao.bachur@mec.gov.br',0,0,'L',0);
		// ---------------- Fim Em caso de Dùvida ----------------
		
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	
			(
				arqnome,
				arqextensao,
				arqdescricao,
				arqtipo,
				arqtamanho,
				arqdata,
				arqhora,
				usucpf,
				sisid
			)VALUES(
				'$ntcid',
				'pdf',
				'Nota Técnica',
				'application/pdf',
				'0',
				'".date('Y-m-d')."',
				'".date('H:i:s')."',
				'".$_SESSION["usucpf"]."',
				". $_SESSION["sisid"] ."
			) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	//Insere o arqid na tabela academico.notatecnica
	$sql = "UPDATE academico.notatecnica
			SET arqid = '$arqid'
				WHERE ntcid = $ntcid";
	$db->executar($sql);
	$db->commit();
	
	if(!is_dir('../../arquivos')) {
		mkdir(APPRAIZ.'/arquivos', 0777);
	}
	if(!is_dir('../../arquivos/sig')) {
		mkdir(APPRAIZ.'/arquivos/sig', 0777);
	}
	if(!is_dir('../../arquivos/sig/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/sig/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
	$PDF->Output($caminho,F); //saída do PDF
	
}


function monta_lista_simples_notatecnica($sql,$cabecalho="",$perpage,$pages,$soma='N',$largura='95%', $valormonetario='S') {
	global $db;
	if(!(bool)$largura) $largura = '95%';
	// este método monta uma listagem na tela baseado na sql passada
	//Registro Atual (instanciado na chamada)
	if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);

    if (is_array($sql))
        $RS = $sql;
    else
        $RS = $db->carregar($sql);

	$nlinhas = $RS ? count($RS) : 0;
	if (! $RS) $nl = 0; else $nl=$nlinhas;
	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
	print '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
	if ($nlinhas>0)
	{
		//Monta Cabeçalho
		if(is_array($cabecalho))
		{
			print '<thead><tr>';
			for ($i=0;$i<count($cabecalho);$i++)
			{
				print '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].'</label>';
			}
			print '</tr> </thead>';
		}

        echo '<tbody>';

		//Monta Listagem
		$totais = array();
		$tipovl = array();
		for ($i=($numero-1);$i<$reg_fim;$i++)
		{
			$c = 0;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
			foreach($RS[$i] as $k=>$v) {

				if (is_numeric($v))
				{
					//cria o array totalizador
					if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
					//Mostra o resultado
					if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
					if ($v<0) print '<td align="right" title="'.$cabecalho[$c].'">('.$v.')'; else print '<td align="right" title="'.$cabecalho[$c].'">'.$v;
					print ('<br>'.$totais[$c]);
				}
				else print '<td title="'.$cabecalho[$c].'">'.$v;
				print '</td>';
				$c = $c + 1;
			}
			print '</tr>';
		}

        print '</tbody>';

		$somarCampos = $soma!='S' && is_array($soma) && (@count($soma)>0);
		if ($soma=='S' || $somarCampos){
			//totaliza (imprime totais dos campos numericos)
			print '<tfoot><tr>';
			for ($i=0;$i<$c;$i++)
			{
				print '<td align="right" title="'.$cabecalho[$i].'">';

				if ($i==0) print 'Totais:   ';
				if(($somarCampos && $soma[$i]) || $soma=='S') {
					if (is_numeric($totais['0'.$i])) 
						if($valormonetario == 'S'){
							print number_format($totais['0'.$i], 2, ',', '.'); 
						}else{
							print $totais['0'.$i]; 
						}
					else 
						print $totais['0'.$i];
				}
				print '</td>';
			}
			print '</tr></tfoot>';
			//fim totais
		}

	}
	else {
		print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
	}
	print '</table>';
}


function monta_endereco($id) {
	global $db;
	?>
	<tr><td class="SubTituloDireita">Endereço :</td><td>
	<? 
	$linha = $db->pegaLinha("
SELECT ende.*
	FROM
		entidade.endereco as ende
	WHERE ende.entid = '".$id."'");
	$dadosendvazio = true;
	unset($endes);
	if(trim($linha['endlog'])) {
		$endes[] = $linha['endlog'];
		$dadosendvazio = false;
	}
	if(trim($linha['endnum'])) {
		$endes[] = " número ".$linha['endnum'];
		$dadosendvazio = false;
	}
	if(trim($linha['endbai'])) {
		$endes[] = $linha['endbai'];
		$dadosendvazio = false;
	}
	if(trim($linha['estuf'])) {
		$endes[] = $linha['estuf'];
		$dadosendvazio = false;
	}
	if(trim($linha['endcep'])) {
		$endes[] = "CEP ".$linha['endcep'];
		$dadosendvazio = false;
	}
	if($dadosendvazio) {
		echo "Não informado";
	} else {
		echo implode(", ",$endes);
	}
	?></td></tr>
	<tr><td class="SubTituloDireita" nowrap>Municípios limítrofes :</td><td><?
	if($linha['muncod']) {
		unset($dadosvizinhos,$vizs);
		$sql = "SELECT mun.mundescricao FROM territorios.municipiosvizinhos muv 
				LEFT JOIN territorios.municipio mun ON muv.muncodvizinho = mun.muncod 
				WHERE muv.muncod='".$linha['muncod']."'";
		$dadosvizinhos = $db->carregar($sql);
		if($dadosvizinhos[0]) {
			foreach($dadosvizinhos as $viz) {
				$vizs[]= $viz['mundescricao'];
			}
			echo implode(", ", $vizs);
		} else {
			echo "Endereço não informado";
		}
	} else {
		echo "Endereço não informado";
	}
	?></td></tr><?
}


function monta_dirigente($id, $ent) {
	global $db,$_funcoes;
	
	$dirigente = $db->pegaLinha("SELECT ent.* FROM entidade.entidade ent 
								 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
								 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
								 WHERE fea.entid = '".$id."' AND fen.funid='". $_funcoes[$_SESSION['sig_var']['orgid']][$ent] ."'");
	if($dirigente) {
	?>
	<tr><td class="SubTituloDireita">Nome :</td><td><? echo (($dirigente['entnome'])?$dirigente['entnome']:"Não informado"); ?></td></tr>
	<tr><td class="SubTituloDireita" nowrap>Função / Ocupação :</td><td><? echo (($dirigente['fundsc'])?$dirigente['fundsc']:"Não informado");?></td></tr>
	<tr><td class="SubTituloDireita">Telefones :</td><td><? 
	$dadostelvazio = true;
	if($dirigente['entnumresidencial']) {
		$tels[] = "Residencial: (".trim($dirigente['entnumdddresidencial']).") ".trim($dirigente['entnumresidencial']);
		$dadostelvazio = false;
	}
	unset($ramal);
	if($dirigente['entnumcomercial']) {
		if(trim($dirigente['entnumramalcomercial']))
			$ramal = " ramal ".trim($dirigente['entnumramalcomercial']);
		$tels[] = "Comercial: (".trim($dirigente['entnumdddcomercial']).") ".trim($dirigente['entnumcomercial']).$ramal;
		$dadostelvazio = false;
	}
	unset($ramal);
	if($dirigente['entnumfax']) {
		if(trim($dirigente['entnumramalfax']))
			$ramal = " ramal ".trim($dirigente['entnumramalfax']);
		$tels[] = "Fax: (".trim($dirigente['entnumdddfax']).") ".trim($dirigente['entnumfax']).$ramal;
		$dadostelvazio = false;
	}
	if($dadostelvazio) {
		echo "Não informado";
	} else {
		echo implode(", ",$tels);
	}
	?></td></tr>
	<tr><td class="SubTituloDireita">E-mail :</td><td><? echo (($dirigente['entemail'])?$dirigente['entemail']:"Não informado"); ?></td></tr>
	<?
	} else {
	?><tr><td colspan="2">Não existe</td></tr><?
	}
}


if($_REQUEST['gerarnotatecnica']=='sim') {		
	$sql = "UPDATE academico.notatecnica SET ntcstatus = 'I' WHERE entid='".$_SESSION['sig_var']['entid']."'";
	$db->executar($sql);
	$sql = "INSERT INTO academico.notatecnica(
   		        usucpf, entid, ntcconteudo, ntcagproposta,ntcpainaug)
   			VALUES ('".$_SESSION['usucpf']."', '".$_SESSION['sig_var']['entid']."', '','".$_REQUEST['ntcagproposta']."','".$_REQUEST['ntcpainaug']."') RETURNING ntcid;";
	$ntcid = $db->pegaUm($sql);
	salvaPDF($ntcid);

} elseif($_REQUEST['ntcid']) {
	$sql = "SELECT ntcconteudo FROM academico.notatecnica WHERE ntcid='".$_REQUEST['ntcid']."'";
	echo $db->pegaUm($sql);
	exit;
}else {
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="gerarnotatecnica" value="sim">
	<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
	<tr>
	<td class="SubTituloCentro" colspan="2">NOTA TÉCNICA</td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Agenda proposta :</td>
	<td><? echo campo_textarea('ntcagproposta', 'N', 'S', '', '100', '5', '1000'); ?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Parecer do MEC sobre a proposta de inauguração :</td>
	<td><? echo campo_textarea('ntcpainaug', 'N', 'S', '', '100', '5', '1000'); ?></td>
	</tr>
	<tr bgcolor="#C0C0C0">
	<td colspan="2" align="center"><input type="button" onclick="if(document.getElementById('ntcagproposta').value!=''){this.disabled=true;document.getElementById('formulario').submit();}else{alert('Agenda proposta é obrigatória.');return false;}" value="Gerar"></td>
	</tr>
	</table>
	</form>
	<?
	exit;
}

if($_SESSION['sig_var']['iscampus'] == 'sim') {
	$unidadeid = $db->pegaUm("SELECT fea.entid FROM entidade.entidade ent
							  LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
							  LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
							  WHERE ent.entid='".$_SESSION['sig_var']['entid']."'");
	$campusfixo = "AND e.entid ='".$_SESSION['sig_var']['entid']."'";
} elseif($_SESSION['sig_var']['iscampus'] == 'nao') {
	$unidadeid = $_SESSION['sig_var']['entid'];
}
?>
<html>
<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">

		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<style>
		.titulounidade {
			background-color:#CDCDCD;
			border-top:2px solid #000000;
			border-bottom:2px solid #000000;
			text-align:center;
			font-weight: bold;
		}
		</style>
</head>
<body onunload="window.opener.location.reload();">
	<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="notprint">
		<tr bgcolor="#ffffff"> 
			<td width="60"><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
			<td height="20" nowrap>
			MINISTÉRIO DA EDUCAÇÃO<br>
			Gabinete do Ministro - Esplanada dos Ministérios - Bloco "L" 8º andar<br >
			CEP: 70047-900 - Brasília - DF - Brasil<br>
			Tel. (61) 2104.8520 / 2104.8740  - FAX nº (61) 2104.9198
			</td>
			<td align="right"><a style="cursor:pointer;" onclick="window.print();"><img src="../imagens/ico_print.jpg" border="0"></a></td>
		</tr>
	</table>
	<br />
	<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
	<thead class="notscreen"  >
	<th colspan="2">
	<table align="center" width="100%" border="0" cellpadding="0" cellspacing="0">
		<tr bgcolor="#ffffff"> 
			<td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
			<td height="20" nowrap>
			MINISTÉRIO DA EDUCAÇÃO<br>
			Gabinete do Ministro - Esplanada dos Ministérios - Bloco "L" 8º andar<br >
			CEP: 70047-900 - Brasília - DF - Brasil<br>
			Tel. (61) 2104.8520 / 2104.8740  - FAX nº (61) 2104.9198
			</td>
		</tr>
	</table><br/>
	</th>
	</thead>
	
	<tr><td colspan="2" class="SubTituloCentro" style="text-align:center;font-size:14px;">
	
	Nota Técnica Nº <? echo $ntcid; ?> - Data de criação : <? echo $db->pegaUm("SELECT to_char(ntcdatageracao,'dd/mm/yyyy HH24:MI:SS') AS data FROM academico.notatecnica WHERE ntcid='".$ntcid."'"); ?></td></tr>
	<tr><td colspan="2">&nbsp;</td></tr>
	<tr><td class="SubTituloEsquerda" width="15%"><b>Agenda proposta :</b></td><td><? echo $_REQUEST['ntcagproposta']; ?></td></tr>
	<tr><td class="SubTituloEsquerda" width="15%"><b>Parecer do MEC sobre a proposta de inauguração :</b></td><td><? echo $_REQUEST['ntcpainaug']; ?></td></tr>
	<tr><td colspan="2">&nbsp;</td></tr>
	<tr><td colspan="2" class="titulounidade"><? echo $db->pegaUm("SELECT UPPER(entnome) FROM entidade.entidade WHERE entid='".$unidadeid."'"); ?></td></tr>
	
	<tr><td class="SubTituloEsquerda" colspan="2">Caracterização da unidade</td></tr>
	<tr><td colspan="2"><? 
	$edtdsc = $db->pegaUm("SELECT edtdsc FROM academico.entidadedetalhe WHERE entid='".$unidadeid."'");
	if($edtdsc) {
		echo $edtdsc; 
	} else {
		echo "Não informado";
	}
	?></td></tr>
	<tr><td class="SubTituloEsquerda" colspan="2">Dados da unidade</td></tr>
	<? monta_endereco($unidadeid); ?>
		
	<tr><td class="SubTituloEsquerda" colspan="2">Dados do dirigente</td></tr>
	<? monta_dirigente($unidadeid,'unidade'); ?>
	<?
	
	$sql = "SELECT DISTINCT
			 cam.cmpid,
			 cam.cmpobs,
			 cam.cmpdataimplantacao,
			 cam.cmpexistencia,
			 cam.cmpsituacao,
			 cam.cmpinstalacao,
			 cam.cmpsituacaoobra,
			 e.entid as campusid,
    		 e.entnome as campus, 
	    	 en.estuf AS uf,
	    	 to_char(cam.cmpdataatualizacao, 'dd/mm/yyyy HH24:MI:SS') as cmpdataatualizacao,
			 m.mundsc AS municipio
			FROM entidade.entidade e 
			INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
			INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
			INNER JOIN entidade.entidade au ON fea.entid = au.entid 
			INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = au.entid
			INNER JOIN academico.orgaouo teu ON teu.funid = fen2.funid 
			INNER JOIN academico.campus cam ON e.entid = cam.entid					
			INNER JOIN entidade.endereco en ON ( e.entid = en.entid AND en.tpeid = 1)
			INNER JOIN public.municipio m on m.muncod = en.muncod  
			WHERE fea.entid = '". $unidadeid ."' ".$campusfixo;
	$campus = $db->carregar($sql);
	
	if($campus[0]) {
		foreach($campus as $camp) {
			$tipolocalidade = definirtipolocalidade($_SESSION['sig_var']['orgid']);
	?>
	<tr><td colspan="2" class="titulounidade"><? echo strtoupper($camp['campus']); ?></td></tr>
		
	<tr><td class="SubTituloEsquerda" colspan="2">Caracterização d<? echo $tipolocalidade['artigo+nome']; ?></td></tr>
	<tr><td colspan="2"><? 
	$edtdsc = $db->pegaUm("SELECT edtdsc FROM academico.entidadedetalhe WHERE entid='".$camp['campusid']."'");
	if($edtdsc) {
		echo $edtdsc; 
	} else {
		echo "Não informado";
	}
	?></td></tr>
		
	<tr><td class="SubTituloEsquerda" colspan="2">Dados d<? echo $tipolocalidade['artigo+nome']; ?></td></tr>
	<!-- ESCREVENDO O ENDEREÇO -->
	<? monta_endereco($camp['campusid']); ?>
	<!-- FIM - ESCREVENDO O ENDEREÇO -->
	
	<!-- DADOS DO ESPECIFICOS -->
	<tr><td class="SubTituloDireita">Situação do campus/uned :</td>
		<td>
		<? 
		$_cmpexistencia = array("N"=>"Novo","P"=>"Pré-Existente");
		$_cmpsituacao = array("F"=>"Funcionando","N"=>"Não Funcionando");
		$_cmpinstalacao = array("P"=>"Instalações Provisórias","D"=>"Instalações Definitivas");
		echo (($_cmpexistencia[$camp['cmpexistencia']])?$_cmpexistencia[$camp['cmpexistencia']].", ":"Não informado, ");
		echo (($_cmpsituacao[$camp['cmpsituacao']])?$_cmpsituacao[$camp['cmpsituacao']]:"Não informado");
		if($camp['cmpsituacao']=='F') {
			echo " (".$_cmpinstalacao[$camp['cmpinstalacao']].")";
		}
		?>
		</td>
	</tr>
	<!-- FIM - DADOS DO ESPECIFICOS -->
	
	<!-- ESCREVENDO OS DADOS DO DIRIGENTE -->
	<tr><td class="SubTituloEsquerda" colspan="2">Dados do dirigente</td></tr>
	<? monta_dirigente($camp['campusid'],'campus'); ?>
	<!-- FIM - ESCREVENDO OS DADOS DO DIRIGENTE -->

	<!-- ESCREVENDO INFORMAÇÕES SOBRE A ENTIDADE -->
	
	<tr><td class="SubTituloEsquerda" colspan="2">INFORMAÇÕES ACADÊMICAS E FINANCEIRAS</td></tr>
	
	<tr><td colspan="2"><strong>Processo seletivo de estudantes</strong></td></tr>
	<tr><td colspan="2"><?
		$sql = "SELECT 'de '||(to_char(prsinscricaoini,'DD/MM/YYYY') ||' até '|| to_char(prsinscricaofim,'DD/MM/YYYY')) as perinsc, 'de '||(to_char(prsprovaini,'DD/MM/YYYY') ||' até '|| to_char(prsprovafim,'DD/MM/YYYY')) as perprova, to_char(prsinicioaula,'DD/MM/YYYY') FROM academico.campus cam
							  INNER JOIN academico.processoseletivo prs ON prs.cmpid = cam.cmpid  
							  WHERE cam.entid='".$camp['campusid']."'";
		
		$cabecalho = array("Inscrição","Provas","Início das aulas");
		monta_lista_simples_notatecnica( $sql, $cabecalho, 50, 10, 'N', '100%','N');
	?></td></tr>
	<tr><td colspan="2"><strong><? echo (($tituloitens[$_SESSION['sig_var']['orgid']])?$tituloitens[$_SESSION['sig_var']['orgid']]:$tituloitens['default']); ?></strong></td></tr>
	<tr><td colspan="2">
		<?
	// Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso não, utilizar o padrão
	if($anosanalisados[$_SESSION['sig_var']['orgid']]) {
		$anos = $anosanalisados[$_SESSION['sig_var']['orgid']];
	} else {
		$anos = $anosanalisados['default'];
	}
	unset($cabecalho,$paramselects);
	$cabecalho[] = "Itens";
	foreach($anos as $ano) {
		$paramselects[] = "(CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $camp['cmpid'] ."') is null 
							THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $camp['cmpid'] ."') END) AS ano_".$ano;
		$cabecalho[] = $ano;		
	}
	$paramselects = implode(",",$paramselects);
	// criando o SELECT
	$sql = "SELECT '<span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' );\" >'||itm.itmdsc||'</span>',
			". $paramselects ."
			FROM academico.item itm 
			LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
			LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
			WHERE tei.orgid = '". $_SESSION['sig_var']['orgid'] ."' AND itm.itmglobal = false
			ORDER BY tei.teiordem";
	monta_lista_simples_notatecnica( $sql, $cabecalho, 50, 10, 'N', '100%','N');
		
		?>
		</td></tr>
	<tr><td colspan="2"><strong>Situação Atual</strong></td></tr>
	<tr><td colspan="2"><?
		unset($cabecalho);
		$cabecalho = array("Itens", "Atual");
		$paramselct = "(CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $camp['cmpid'] ."') is null 
						THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $camp['cmpid'] ."') END)  AS ano";
	
		$sql = "SELECT '<span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' )\" >'||itm.itmdsc||'</span>',
			". $paramselct ."
			FROM academico.item itm 
			LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
			LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
			WHERE tei.orgid = '". $_SESSION['sig_var']['orgid'] ."' AND itm.itmglobal = true
			ORDER BY tei.teiordem";
		monta_lista_simples_notatecnica( $sql, $cabecalho, 50, 10, 'N', '100%','N');
	?></td></tr>
		<tr><td colspan="2"><strong><? echo (($titulocursos[$_SESSION['sig_var']['orgid']])?$titulocursos[$_SESSION['sig_var']['orgid']]:$titulocursos['default']); ?></strong></td></tr>
		<tr>
		<td colspan="2">
		
		<?
	unset($cabecalho,$inputs,$totalizador);
	$cabecalho[] = "Tipo de curso";
	$cabecalho[] = "Cursos";
	if($anosanalisados[$_SESSION['sig_var']['orgid']]) {
		$anos = $anosanalisados[$_SESSION['sig_var']['orgid']];
	} else {
		$anos = $anosanalisados['default'];
	}
	// Pegando mascara definida para a quantidade de cursos (constantes.php)
	$mask = $db->carregar("SELECT tpimascara,tpitamanhomax FROM academico.tipoitem WHERE tpiid='".TIPOITEM_QTD."'");
	if($mask) {
		$mask = current($mask);
	}
	foreach($anos as $ano) {
		$cabecalho[] = $ano;
		$inputs[] = "(SELECT coalesce(cast(cpcqtd as varchar),'') FROM academico.campuscurso cmc1 WHERE cmc1.curid = cmc2.curid AND cmc1.cpcano='".$ano."' AND cmc1.cmpid='".$camp['cmpid']."') as ano".$ano;
		$totalizador[$ano] = "<input type='text' size='12' class='normal' name='tot".$ano."' value='' readonly>";
	}
	
	$sql = "SELECT tpc.tpcdsc, cur.curdsc as curso, ".implode(",",$inputs)." FROM academico.campuscurso cmc2 
			LEFT JOIN public.curso cur ON cur.curid = cmc2.curid 
			LEFT JOIN public.tipocurso tpc ON tpc.tpcid = cur.tpcid 
			WHERE cmc2.cmpid='".$camp['cmpid']."' GROUP BY cmc2.curid, cur.curdsc, tpc.tpcdsc";
	$regcur = $db->carregar($sql);
	if($_SESSION['sig_var']['orgid']==TPENSSUP) {
		unset($dadosdif,$dadosnotdif);
		echo '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
		if($regcur[0]) {
			// Analisando quais cursos tiveram suas quantidade modificadas nos anos (solicitado pelo Hugo)		
			foreach($regcur as $cur) {
				$diff = false;
				$dant = current($cur);
				foreach($cur as $d) {
					if(is_numeric($dant) && is_numeric($d) && $d != $dant) {
						$diff = true;
					}
					$dant = $d; 
				}
				if($diff) {
					$dadosdif[] = $cur;	
				} else {
					$dadosnotdif[] = $cur;
				}
			}
			echo '<thead>';
			echo '<tr>';
			foreach($cabecalho as $cabec) {
				echo '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabec.'</td>';	
			}
		  	echo '</tr>';
		  	echo '</thead>';
	  		if($dadosdif[0]) {
			  	echo '<tr><td colspan="'.count($cabecalho).'"><b>Cursos com alterações na quantidade de vagas no decorrer dos anos</b></td></tr>';
				foreach($dadosdif as $cur) {
					echo '<tr>';
					foreach($cur as $d) {
						echo '<td '.((is_numeric($d))?'align="right"':'').'>'.$d.'</td>';
					}
					echo '</tr>';
				}
		  	}
		  	if($dadosnotdif[0]) {
			  	echo '<tr><td colspan="'.count($cabecalho).'"><b>Cursos sem alterações na quantidade de vagas no decorrer dos anos</b></td></tr>';
				foreach($dadosnotdif as $cur) {
					echo '<tr>';
					foreach($cur as $d) {
						echo '<td '.((is_numeric($d))?'align="right"':'').'>'.$d.'</td>';
					}
					echo '</tr>';
				}
		  	}
		} else {
			echo '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
		}
		echo '</table>';
	} else {
		monta_lista_simples_notatecnica( $sql, $cabecalho, 50, 10, 'N', '100%','N');
	}
	?>
	</td>
	</tr>
	<!-- FIM - ESCREVENDO INFORMAÇÕES SOBRE A ENTIDADE -->
		<tr>
		<td class="SubTituloEsquerda" colspan="2">INFORMAÇÕES SOBRE OBRAS</td>
		</tr>
		<tr><td class="SubTituloDireita" nowrap>Situação sobre obras no campus/uned :</td><td><? $_cmpsituacaoobra = array("L"=>"Licitação de Obras","A"=>"Obras em Andamento","C"=>"Obras Concluídas"); echo (($_cmpsituacaoobra[$camp['cmpsituacaoobra']])?$_cmpsituacaoobra[$camp['cmpsituacaoobra']]:"Não informado"); ?></td></tr>
		<tr><td class="SubTituloEsquerda" colspan="2"><b>Obra(s) a ser(em) inaugurada(s)</b></td></tr>
		<tr><td colspan="2">
		<table width="100%" cellSpacing="1" cellPadding="3" align="center">
		<?
		$sql = "SELECT obr.obrid, obr.obrdesc, obr.obrdtinicio, obr.obrdttermino, mun.mundescricao||'/'||en.estuf as local, obr.obrcomposicao, sto.stodesc FROM academico.campus cam 
				LEFT JOIN obras.obrainfraestrutura obr ON obr.entidcampus = cam.entid  
				LEFT JOIN entidade.endereco en ON en.endid = obr.endid 
				LEFT JOIN territorios.municipio mun ON mun.muncod = en.muncod 
				LEFT JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
				LEFT JOIN academico.obrainauguradacampus obi ON obr.obrid = obi.obrid 
				WHERE cam.cmpid='".$camp['cmpid']."' AND obr.obsstatus='A' AND obi.obcid IS NOT NULL AND (obr.obrstatusinauguracao ='N' OR obr.obrstatusinauguracao IS NULL) ORDER BY sto.stodesc";
		$obraaserinaugurada = $db->carregar($sql);
		if($obraaserinaugurada[0]) {
			foreach($obraaserinaugurada as $obr) {
		?>
		<tr>
			<td class="SubTituloDireita">Nome da obra :</td><td colspan="2"><b><? echo $obr['obrdesc']; ?></b></td>
			<td class="SubTituloDireita">Município/UF :</td><td><? echo $obr['local']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Início programado :</td><td colspan="2"><? echo formata_data($obr['obrdtinicio']); ?></td>
			<td class="SubTituloDireita">Término programado :</td><td><? echo formata_data($obr['obrdttermino']); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição :</td><td colspan="4"><? echo (($obr['obrcomposicao'])?nl2br($obr['obrcomposicao']):"Nenhuma observação inserida"); ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="5">Fotos</td>
		</tr>
		<? 
				$sql = "SELECT arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao, to_char(oar.aqodtinclusao,'dd/mm/yyyy') as aqodtinclusao FROM public.arquivo arq
						INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
						WHERE oar.obrid='". $obr['obrid'] ."' AND
		  					aqostatus = 'A' AND
		  				   (arqtipo = 'image/jpeg' OR 
		   		 			arqtipo = 'image/gif' OR 
		   		 			arqtipo = 'image/png') 
						ORDER BY arq.arqid DESC LIMIT 5";
				$fotos = $db->carregar($sql);
				echo "<tr>";
				if($fotos[0]){
					for($k=0;$k < count($fotos);$k++){
						echo "<td valign=\"top\" align=\"center\">";
						echo "<img id='".$fotos[$k]["arqid"]."' src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras&newwidth=120&newheight=90&arqid=".$fotos[$k]["arqid"]."' hspace='10' vspace='3' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\"/><br />";
						echo $fotos[$k]["aqodtinclusao"]."<br />";
						echo $fotos[$k]["arqdescricao"];
						echo "</td>";
					}
				} else {
					echo "<td colspan='5'>Não existe(m) foto(s) cadastrada(s)</td>";
				}
				echo "</tr>";
			}
		} else {
			echo "<tr><td><b>Não existe(m) Obra(s) a ser(em) inaugurada(s).</b></td></tr>";
		}
		?>
		</table>
		</td></tr>
		<tr><td class="SubTituloEsquerda" colspan="2">Outra(s) obra(s) do campus/uned</td></tr>
		<tr><td colspan="2">
		<table width="100%" cellSpacing="1" cellPadding="3" align="center">
		<?
		$sql = "SELECT obr.obrid, obr.obrdesc, obr.obrdtinicio, obr.obrdttermino, mun.mundescricao||'/'||en.estuf as local, obr.obrcomposicao, sto.stodesc FROM academico.campus cam 
				LEFT JOIN obras.obrainfraestrutura obr ON obr.entidcampus = cam.entid  
				LEFT JOIN entidade.endereco en ON en.endid = obr.endid 
				LEFT JOIN territorios.municipio mun ON mun.muncod = en.muncod 
				LEFT JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
				LEFT JOIN academico.obrainauguradacampus obi ON obr.obrid = obi.obrid 
				WHERE cam.cmpid='".$camp['cmpid']."' AND obr.obsstatus='A' AND obi.obcid IS NULL AND (obr.obrstatusinauguracao ='N' OR obr.obrstatusinauguracao IS NULL) ORDER BY sto.stodesc";
		$obraaserinaugurada = $db->carregar($sql);
		if($obraaserinaugurada[0]) {
			foreach($obraaserinaugurada as $obr) {
		?>
		<tr>
			<td class="SubTituloDireita">Nome da obra :</td><td colspan="2"><b><? echo $obr['obrdesc']; ?></b></td>
			<td class="SubTituloDireita">Município/UF :</td><td><? echo $obr['local']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Início programado :</td><td colspan="2"><? echo formata_data($obr['obrdtinicio']); ?></td>
			<td class="SubTituloDireita">Término programado :</td><td><? echo formata_data($obr['obrdttermino']); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição :</td><td colspan="4"><? echo (($obr['obrcomposicao'])?nl2br($obr['obrcomposicao']):"Não existem observações sobre a obra"); ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="5">Fotos</td>
		</tr>
		<? 
				$sql = "SELECT arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao, to_char(oar.aqodtinclusao,'dd/mm/yyyy') as aqodtinclusao FROM public.arquivo arq
						INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
						INNER JOIN obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
						WHERE obr.obrid='". $obr['obrid'] ."' AND
		  					aqostatus = 'A' AND
		  				   (arqtipo = 'image/jpeg' OR 
		   		 			arqtipo = 'image/gif' OR 
		   		 			arqtipo = 'image/png') 
						ORDER BY arq.arqid DESC LIMIT 5";

				$fotos = $db->carregar($sql);
				echo "<tr>";
				if($fotos[0]){
					for($k=0;$k < count($fotos);$k++){
						echo "<td valign=\"top\" align=\"center\">";
						$_SESSION['imgparametos'][$fotos[$k]["arqid"]] = array("filtro" => "cnt.obrid=".$uni['obrid']." AND aqostatus = 'A'", "tabela" => "obras.arquivosobra");
						echo "<img id='".$fotos[$k]["arqid"]."' src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras&newwidth=120&newheight=90&arqid=".$fotos[$k]["arqid"]."' hspace='10' vspace='3' style='width:80px; height:80px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\"/><br />";
						echo $fotos[$k]["aqodtinclusao"]."<br />";
						echo $fotos[$k]["arqdescricao"];
						echo "</td>";
					}
				} else {
					echo "<td colspan='5'>Não existe(m) foto(s) cadastrada(s).</td>";
				}
				echo "</tr>";
			}
		} else {
			echo "<tr><td><b>Não existe(m) outra(s) obra(s) a ser(em) inaugurada(s)</b></td></tr>";
		}
		?>
		</table>
		</td></tr>
		<tr><td class="SubTituloEsquerda" colspan="2">Informações Adicionais</td></tr>
		<tr><td colspan="2"><? echo (($camp['cmpobs'])?$camp['cmpobs']:"Não informado"); ?></td></tr>
		<tr><td colspan="2">&nbsp;</td></tr>
		<?
		}
		
	}
	// Carregando os dados sobre a nota técnica, diferencia unidade / campus
	if($_SESSION['sig_var']['iscampus'] == 'sim') {
		$sql = "SELECT to_char(cam.cmpdataatualizacao, 'dd/mm/yyyy HH24:MI:SS') as cmpdataatualizacao, 
						usu.usunome, 
						usu.usufonenum, 
						usu.usufoneddd, 
						usu.usuemail 
				FROM academico.campus cam 
				LEFT JOIN seguranca.usuario usu ON usu.usucpf = cam.usucpf
				WHERE cam.entid = '".$_SESSION['sig_var']['entid']."'";
		
	} elseif($_SESSION['sig_var']['iscampus'] == 'nao') {
		$sql = "SELECT to_char(cam.cmpdataatualizacao, 'dd/mm/yyyy HH24:MI:SS') as cmpdataatualizacao, 
						usu.usunome, 
						usu.usufonenum, 
						usu.usufoneddd, 
						usu.usuemail 
				FROM academico.campus cam 
				LEFT JOIN seguranca.usuario usu ON usu.usucpf = cam.usucpf 
				LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid 
				LEFT JOIN entidade.funcaoentidade fen ON fen.entid  = ent.entid 
				LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
				LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid  
				WHERE uor.entid = '".$unidadeid."' ORDER BY cmpdataatualizacao DESC LIMIT 1";
	}
	$sobrenotatec = $db->pegaLinha($sql);
	
	?>
	<tr><td class="SubTituloEsquerda" colspan="2">Sobre a Nota Técnica</td></tr>
	<tr><td class="SubTituloDireita">Data da última atualização :</td><td><? echo (($sobrenotatec['cmpdataatualizacao'])?$sobrenotatec['cmpdataatualizacao']:"Não atualizado"); ?></td></tr>
	<tr><td class="SubTituloDireita">Técnico responsável :</td><td><? echo (($sobrenotatec['usunome'])?$sobrenotatec['usunome']:"Não atualizado"); ?></td></tr>
	<tr><td class="SubTituloDireita">Telefone :</td><td><?
	if($sobrenotatec['usufonenum']) { 
		echo "(".$sobrenotatec['usufoneddd'].") ".$sobrenotatec['usufonenum'];
	} else {
		echo "Não atualizado";
	}
	?>
	</td></tr>
	<tr><td class="SubTituloDireita">E-mail :</td><td><? echo (($sobrenotatec['usuemail'])?$sobrenotatec['usuemail']:"Não atualizado"); ?></td></tr>

	<tr><td class="SubTituloEsquerda" colspan="2">EM CASO DE DÚVIDAS CONTATAR</td></tr>
	<tr><td colspan="2">
	João Paulo Bachur<br />
	Chefe de Gabinete do Ministro de Estado da Educação<br />
	Telefones: (61) 2104-8740<br />
	E-mail: joao.bachur@mec.gov.br
	</td></tr>
	</table>

	</body>
	</html>
<?

	$sql = "UPDATE academico.notatecnica SET ntcconteudo='".addslashes(ob_get_contents())."' WHERE ntcid='".$ntcid."'";
	$db->executar($sql);
	$db->commit();
	ob_end_clean();
	echo "<script>
			window.location= '?modulo=principal/visualizarnotatecnica&acao=A&ntcid=".$ntcid."';
		  </script>";
?>