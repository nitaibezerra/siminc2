<?php

class CampoExternoControle{

	private $htm;
	private static $camposDefault;

	//Função obrigatória. Nessa função deverá ser montado a questão que aparecerá dentro do campo externo. 
	//Deverá ser colocada dentro do $this->htm.
	public function montaNovoCampo( $perid, $qrpid, $percent = 90 ){
		global $db;
		$obQuestionario = new QQuestionarioResposta();

		$queid = $obQuestionario->pegaQuestionario( $qrpid );

		$bgcolor = "#A8A8A8";
		
		if(in_array($perid, array(3578))){
			$funcao = 'codigoInep';
		}
		
		if(in_array($perid, array(PERID_NT_ATENDIMENTO_EXTERNO1))){
			$funcao = 'numAtualMatriculasNT';
		}

		if(in_array($perid, array(PERID_NT_ATENDIMENTO_EXTERNO2))){
			$funcao = 'numAtualTurmasNT';
		}
		
		if(method_exists($this, $funcao)){
			$data['perid'] = $perid;
			$data['queid'] = $queid;
			$data['qrpid'] = $qrpid;			
			$htm = self::$funcao($data); 
		}else{
			$htm = self::perguntaDefault($perid, $qrpid, $percent);
		}	

	  	$this->htm = $htm;
	}
	
	public function codigoInep($data = null){
		global $db;
		
		extract($data);
		
		$codigoinep = $db->pegaUm("SELECT qcicodigoinep FROM proinfantil.questionariocodigoinep WHERE perid = 3578 AND qrpid = ".$qrpid);
		$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
		$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
		$htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'>";
		$htm .= "<input type='hidden' name='id' id='salvarinep' value='1'>";
		$htm .= "<table class='tabela' style='width:95%;' align='center'>";
		$htm .= "<tr>";
		$htm .= "<td>".campo_texto('codigoinep', 'S', 'S', '', 70, 70, '########', '', '', '', '', 'id="codigoinep"' ,'', $codigoinep, 'validaInep(this);')."</td>";
		$htm .= "</tr>";
		$htm .= "</table>";
		
		return $htm;
	}
	
	public function numAtualMatriculasNT($data = null){
		global $db;

		//Gravar em: proinfantil.novasturmasdadosmunicipios
		
		$sql = "SELECT 
					COALESCE(SUM(ntcqtdalunocrecheparcialpublica),0) as ntcqtdalunocrecheparcialpublica,
					COALESCE(SUM(ntcqtdalunocrecheintegralpublica),0) as ntcqtdalunocrecheintegralpublica,
					COALESCE(SUM(ntcqtdalunopreescolaparcialpublica),0) as ntcqtdalunopreescolaparcialpublica,
					COALESCE(SUM(ntcqtdalunopreescolaintegralpublica),0) as ntcqtdalunopreescolaintegralpublica,
					COALESCE(SUM(ntcqtdalunocrecheparcialconveniada),0) as ntcqtdalunocrecheparcialconveniada,
					COALESCE(SUM(ntcqtdalunocrecheintegralconveniada),0) as ntcqtdalunocrecheintegralconveniada,
					COALESCE(SUM(ntcqtdalunopreescolaparcialconveniada),0) as ntcqtdalunopreescolaparcialconveniada,
					COALESCE(SUM(ntcqtdalunopreescolaintegralconveniada),0) as ntcqtdalunopreescolaintegralconveniada,
					ntmid,					
					ntmqtdmatriculacrecheparcialpublica,
					ntmqtdmatriculacrecheintegralpublica,
					ntmqtdmatriculapreescolaparcialpublica,
					ntmqtdmatriculapreescolaintegralpublica,
					ntmqtdmatriculacrecheparcialconveniada,
					ntmqtdmatriculacrecheintegralconveniada,
					ntmqtdmatriculapreescolaparcialconveniada,
					ntmqtdmatriculapreescolaintegralconveniada					
				FROM proinfantil.novasturmasdadoscenso dc
				LEFT JOIN proinfantil.novasturmasdadosmunicipios dm on dm.muncod = dc.muncod and ntmano = '".($_SESSION['exercicio'])."'		
				WHERE dc.muncod = '{$_SESSION['proinfantil']['muncod']}'
				AND dc.ntcanocenso = '".($_SESSION['exercicio']-1)."'
				and dc.ntcstatus = 'A'
				GROUP BY
					ntmid,
					ntmqtdmatriculacrecheparcialpublica,
					ntmqtdmatriculacrecheintegralpublica,
					ntmqtdmatriculapreescolaparcialpublica,
					ntmqtdmatriculapreescolaintegralpublica,
					ntmqtdmatriculacrecheparcialconveniada,
					ntmqtdmatriculacrecheintegralconveniada,
					ntmqtdmatriculapreescolaparcialconveniada,
					ntmqtdmatriculapreescolaintegralconveniada";
		//dbg($sql);
		$rs = $db->pegaLinha($sql);
		
		$htm = '
				<input type=\'hidden\' name=\'perid\' id=\'perid\' value=\''.$data['perid'].'\'>
				<input type=\'hidden\' name=\'ntmid\' id=\'ntmid\' value=\''.$rs['ntmid'].'\'>
				<input type=\'hidden\' name=\'qrpid\' id=\'qrpid\' value=\''.$data['qrpid'].'\'>
				<input type=\'hidden\' name=\'identExterno\' id=\'identExterno\' value=\'1\'>
				<input type=\'hidden\' name=\'ntmano\' id=\'ntmano\' value=\''.($_SESSION['exercicio']).'\'>
				<table border="1" bordercolor="#FFFFFF" width="100%" class="listagem">
					<thead>
						<tr>
							<td rowspan="2">&nbsp;</td>
							<th colspan="2">Educacenso 2011</th>
							<th colspan="2">Matrículas em 2012 - SITUAÇÃO ATUAL</th>
						</tr>
						<tr>
							<th>Rede pública municipal</th>
							<th>Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</th>
							<th>Rede pública municipal</th>
							<th>Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</th>
						</tr>
					</thead>
					<tbody>		
						<tr>
							<td align="center">Creche Integral</td>
							<td align="center">'.$rs['ntcqtdalunocrecheintegralpublica'].'</td>
							<td align="center">'.$rs['ntcqtdalunocrecheintegralconveniada'].'</td>
							<td align="center">'.campo_texto('ntmqtdmatriculacrecheintegralpublica', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdmatriculacrecheintegralpublica']).'</td>
							<td align="center">'.campo_texto('ntmqtdmatriculacrecheintegralconveniada', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdmatriculacrecheintegralconveniada']).'</td>
						</tr>
						<tr>
							<td align="center">Creche Parcial</td>
							<td align="center">'.$rs['ntcqtdalunocrecheparcialpublica'].'</td>
							<td align="center">'.$rs['ntcqtdalunocrecheparcialconveniada'].'</td>
							<td align="center">'.campo_texto('ntmqtdmatriculacrecheparcialpublica', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdmatriculacrecheparcialpublica']).'</td>
							<td align="center">'.campo_texto('ntmqtdmatriculacrecheparcialconveniada', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdmatriculacrecheparcialconveniada']).'</td>
						</tr>
						<tr>
							<td align="center">Pré-escola Integral</td>
							<td align="center">'.$rs['ntcqtdalunopreescolaintegralpublica'].'</td>
							<td align="center">'.$rs['ntcqtdalunopreescolaintegralconveniada'].'</td>
							<td align="center">'.campo_texto('ntmqtdmatriculapreescolaintegralpublica', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdmatriculapreescolaintegralpublica']).'</td>
							<td align="center">'.campo_texto('ntmqtdmatriculapreescolaintegralconveniada', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdmatriculapreescolaintegralconveniada']).'</td>
						</tr>
						<tr>
							<td align="center">Pré-escola Parcial</td>
							<td align="center">'.$rs['ntcqtdalunopreescolaparcialpublica'].'</td>
							<td align="center">'.$rs['ntcqtdalunopreescolaparcialconveniada'].'</td>
							<td align="center">'.campo_texto('ntmqtdmatriculapreescolaparcialpublica', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdmatriculapreescolaparcialpublica']).'</td>
							<td align="center">'.campo_texto('ntmqtdmatriculapreescolaparcialconveniada', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdmatriculapreescolaparcialconveniada']).'</td>
						</tr>
					</tbody>
				</table>';
		
		return $htm;
	} 
	
	public function numAtualTurmasNT($data = null){
		global $db;
		
		$sql = "SELECT 
					COALESCE(SUM(ntcqtdturmacrechepublica),0) as ntcqtdturmacrechepublica,
					COALESCE(SUM(ntcqtdturmapreescolapublica),0) as ntcqtdturmapreescolapublica,
					COALESCE(SUM(ntcqtdturmaunificadapublica),0) as ntcqtdturmaunificadapublica,
					COALESCE(SUM(ntcqtdturmacrecheconveniada),0) as ntcqtdturmacrecheconveniada,
					COALESCE(SUM(ntcqtdturmapreescolaconveniada),0) as ntcqtdturmapreescolaconveniada,
					COALESCE(SUM(ntcqtdturmaunificadaconveniada),0) as ntcqtdturmaunificadaconveniada,
					ntmid,
					ntmqtdturmacrechepublica,
					ntmqtdturmapreescolapublica,
					ntmqtdturmaunificadapublica,
					ntmqtdturmacrecheconveniada,
					ntmqtdturmapreescolaconveniada,
					ntmqtdturmaunificadaconveniada				
				FROM proinfantil.novasturmasdadoscenso dc
				LEFT JOIN proinfantil.novasturmasdadosmunicipios dm on dm.muncod = dc.muncod and ntmano = '".($_SESSION['exercicio'])."'
				WHERE dc.muncod = '{$_SESSION['proinfantil']['muncod']}'
				AND dc.ntcanocenso = '".($_SESSION['exercicio']-1)."'
				and dc.ntcstatus = 'A'
				GROUP BY
					ntmid,
					ntmqtdturmacrechepublica,
					ntmqtdturmapreescolapublica,
					ntmqtdturmaunificadapublica,
					ntmqtdturmacrecheconveniada,
					ntmqtdturmapreescolaconveniada,
					ntmqtdturmaunificadaconveniada";
		$rs = $db->pegaLinha($sql);
		
		
		$sql_turmas = "SELECT 	resdsc 
					   FROM 	questionario.resposta 
					   WHERE 	perid = {$data['perid']} AND qrpid = {$data['qrpid']};";
		
		$rs_turmas = $db->pegaLinha($sql_turmas);
		
		$rsMatriculasTurmas = recuperarDadosMatriculasTurmas();

		if($rsMatriculasTurmas){
			$qtdCreche    = $rsMatriculasTurmas['ntmqtdturmacrechepublica']-$rsMatriculasTurmas['ntcqtdturmacrechepublica'];
			$qtdCrecheC   = $rsMatriculasTurmas['ntmqtdturmacrecheconveniada']-$rsMatriculasTurmas['ntcqtdturmacrecheconveniada'];
			$totalCreche = $qtdCreche + $qtdCrecheC;
			
			$qtdPreEscola  = $rsMatriculasTurmas['ntmqtdturmapreescolapublica']-$rsMatriculasTurmas['ntcqtdturmapreescolapublica'];
			$qtdPreEscolaC = $rsMatriculasTurmas['ntmqtdturmapreescolaconveniada']-$rsMatriculasTurmas['ntcqtdturmapreescolaconveniada'];
			$totalPreEscola = $qtdPreEscola + $qtdPreEscolaC;			
			
			$qtdUnificada  = $rsMatriculasTurmas['ntmqtdturmaunificadapublica']-$rsMatriculasTurmas['ntcqtdturmaunificadapublica'];
			$qtdUnificadaC = $rsMatriculasTurmas['ntmqtdturmaunificadaconveniada']-$rsMatriculasTurmas['ntcqtdturmaunificadaconveniada'];
			$totalUnificada = $qtdUnificada + $qtdUnificadaC;			
		} else {
			$totalCreche = "";
			$totalPreEscola = "";
			$totalUnificada = "";
		}
		
		if(!empty($rs_turmas) && ($totalCreche == 0 && $totalPreEscola == 0 && $totalUnificada == 0)){
			$turma = '<table border="0" bordercolor="#FFFFFF" bordercolor="#FFFFFF" width="100%" height="250">
					  	<thead>
							<tr>
								<td align="justify"><font size="2"><b>"Atenção! De acordo com as informações inseridas no "Questionário de Atendimento" 
									o seu município não tem direito ao recurso. Para cadastrar as novas turmas  é necessário que, 
									além de criar novas turmas com todas as matrículas não computadas no âmbito do Fundeb, o município 
									demonstre a ampliação do acesso à educação infantil".</b></font></td>
							</tr>
						</thead>
					 </table>';
		} else {
			$turma = "";
		}
		
		$htm = '
				<input type=\'hidden\' name=\'perid\' id=\'perid\' value=\''.$data['perid'].'\'>
				<input type=\'hidden\' name=\'qrpid\' id=\'qrpid\' value=\''.$data['qrpid'].'\'>
				<input type=\'hidden\' name=\'ntmid\' id=\'ntmid\' value=\''.$rs['ntmid'].'\'>
				<input type=\'hidden\' name=\'identExterno\' id=\'identExterno\' value=\'1\'>
				<input type=\'hidden\' name=\'ntmano\' id=\'ntmano\' value=\''.($_SESSION['exercicio']).'\'>
				<table border="1" bordercolor="#FFFFFF" bordercolor="#FFFFFF" width="100%" class="listagem">
					<thead>
						<tr>
							<td>&nbsp;</td>
							<th colspan="2">Educacenso 2011 - Número de Turmas</th>
							<th colspan="2">Turmas em 2012 - SITUAÇÃO ATUAL</th>
						</tr>
						<tr>
							<td>'.$_SESSION['proinfantil']['muncod'].'&nbsp;</td>
							<th>Rede pública municipal</th>
							<th>Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</th>
							<th>Rede pública municipal</th>
							<th>Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</th>
						</tr>
					</thead>
					<tbody>		
						<tr>
							<td>Creche</td>
							<td align="center">'.$rs['ntcqtdturmacrechepublica'].'</td>
							<td align="center">'.$rs['ntcqtdturmacrecheconveniada'].'</td>
							<td align="center">'.campo_texto('ntmqtdturmacrechepublica', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdturmacrechepublica']).'</td>
							<td align="center">'.campo_texto('ntmqtdturmacrecheconveniada', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdturmacrecheconveniada']).'</td>
						</tr>
						<tr>
							<td>Pré-escola</td>
							<td align="center">'.$rs['ntcqtdturmapreescolapublica'].'</td>
							<td align="center">'.$rs['ntcqtdturmapreescolaconveniada'].'</td>
							<td align="center">'.campo_texto('ntmqtdturmapreescolapublica', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdturmapreescolapublica']).'</td>
							<td align="center">'.campo_texto('ntmqtdturmapreescolaconveniada', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdturmapreescolaconveniada']).'</td>
						</tr>
						<tr>
							<td align="center">Unificada (matrículas de creche e pré-escola na mesma turma)</td>
							<td align="center">'.$rs['ntcqtdturmaunificadapublica'].'</td>
							<td align="center">'.$rs['ntcqtdturmaunificadaconveniada'].'</td>
							<td align="center">'.campo_texto('ntmqtdturmaunificadapublica', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdturmaunificadapublica']).'</td>
							<td align="center">'.campo_texto('ntmqtdturmaunificadaconveniada', 'N', 'S', '', 10, 6, '######', '',    '','','','','',$rs['ntmqtdturmaunificadaconveniada']).'</td>
						</tr>		
					</tbody>
				</table>'.$turma;
		return $htm;
	}
	
	public function perguntaDefault($perid, $qrpid, $percent){
		global $db;
		$sql = "select
					e.entnome
				from
					entidade.entidade e
				INNER JOIN proinfantil.proinfantilescola pie on pie.entid = e.entid
				INNER JOIN proinfantil.questionario q on q.pinid = pie.pinid
				WHERE q.qrpid = ".$qrpid;
		
		$escolaSelecionada = $db->pegaUm( $sql );
		
		$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
		$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
		$htm .= "<input type='hidden' name='identFormulario' id='identFormulario' value='1'>";
		$htm .= "<input type=\"hidden\" name=\"muncod_\" class=\"CampoEstilo\" id=\"muncod_\" value=\"".$arDados['muncod']."\" size=\"20\" maxlength=\"100\" />";
		$htm .= "<table class='tabela' style='width:{$percent}%;' align='center'>";
		$htm .= "<tr>";
		$htm .= "<td>";
		$htm .= "<font style='font-family: Arial; color: black; font-size: 12px;'><b>Selecione a Escola. <img src=\"/imagens/gif_inclui.gif\" style=\"cursor:pointer;\" border=0 title=\"Selecionar Escola\" onclick=\"janela('?modulo=principal/popupSelecionaEscolas&acao=A&qrpid={$qrpid}', 600, 480);\"></b></font><br>";
		$htm .= "<font style='font-family: Arial; color: black; font-size: 12px;'>Nome da Escola Vinculada: ".campo_texto( 'nomeescola', 'N', 'N', '', 70, 70, '####', '', '', '', '', 'id="nomeescola"' ,'' , $escolaSelecionada )."</font><br>";
		$htm .= "</td>";
		$htm .= "</tr>";
		$htm .= "</table>";
		
		return $htm;
	}

	//Da echo no resultado do campo externo para alimentar o relatorio de respostas do questionario.
	//Sem essa função o relatório apenas exibirá o texto "Campo Externo".
	function retornoRelatorio( $qrpid, $perid ){
		$tp = $perid == 935 ? 1 : 2;
		if( $tp == 2 ){
			$obQuest = new QuestoesPontuaisAnexos( );
			$dadosResp = $obQuest->pegaResposta( $qrpid, $perid );
			echo $dadosResp['arqdescricao'];
		} else {
			$obQuest = new QuestoesPontuaisEscolas( );
			echo implode(", <br />", $obQuest->nomeEscolasCadastradas( $qrpid, $perid ) );
		}
	}

	//Função que salva o resultado do campo externo. Não deverá ser salvo no esquema do questionário, e sim no esquema do próprio sistema.
	function salvar(){
		
		$pergunta1 = 'per_'.PERID_NT_ATENDIMENTO_EXTERNO1;
		$pergunta2 = 'per_'.PERID_NT_ATENDIMENTO_EXTERNO2;
			
		if($_REQUEST['csarvore'] == $pergunta1){
			$this->salvaAtualMatriculasNT();
		} else if($_REQUEST['csarvore'] == $pergunta2){
			$this->salvaAtualTurmasNT();
		} else {
			$this->salvaDefault();
		}
	}
	
	function salvarCodInep(){
		global $db;

		$respNao = '3043';
		$respSim = '3044';
		$pergAnterior = '1588'; //id da pergunta 2 - Estabelecimento está cadastrado no Educacenso?
		$entcodent = $_POST['codigoinep'] ? $_POST['codigoinep']: $_POST['entcodent'];
		
		$resposta = $db->pegaUm("select itpid from questionario.resposta where perid = {$pergAnterior} and qrpid = {$_POST['qrpid']}");
		
		if( empty($resposta) ){
			$retorno = '1';
		} else {
			if( $resposta == $respSim ){ 
				//Verifica se existe escola
				$sql_v = "SELECT et.entid FROM
							entidade.entidade et
							INNER JOIN entidade.endereco ed ON et.entid = ed.entid
						WHERE et.entcodent IS NOT NULL
							AND et.entstatus = 'A'
							AND et.entcodent = '{$entcodent}'
							AND ed.muncod = '{$_SESSION['proinfantil']['muncod']}'";
				
				$rs_v = $db->pegaLinha($sql_v);
				
				if(empty($rs_v['entid'])){
					$retorno = '2';
				}
			} elseif( $resposta == $respNao ){
				
				if( strlen($entcodent) != 8 && $entcodent != ''){ 
					$retorno = '3';
				}
			}
		}
		
		if( !empty($_POST['codigoinep']) && empty($retorno) ){
			$db->executar("DELETE FROM proinfantil.questionariocodigoinep WHERE perid = 3578 and qrpid = {$_POST['qrpid']}");
			$sql = "INSERT INTO proinfantil.questionariocodigoinep(usucpf, perid, qrpid, qcicodigoinep, qcidata)
					VALUES('".$_SESSION['usucpf']."',3578,".$_POST['qrpid'].",".$_POST['codigoinep'].",'NOW()' )";
			$db->executar($sql);
			$db->commit();
		}
	}

	//Função obrigatória. Dá echo no $this->htm para imprimir o campo externo na tela.
	function show(){
		echo $this->htm;
	}
	
	function salvaAtualMatriculasNT(){
		global $db;
		
		$muncod 									= $_SESSION['proinfantil']['muncod'];
		$ntmano 									= $_REQUEST['ntmano'];
		$ntmid										= $_REQUEST['ntmid'];
		$ntmsemestre 								= $_REQUEST['ntmsemestre'] ? $_REQUEST['ntmsemestre'] : '';
		$ntmqtdmatriculacrecheparcialpublica		= $_REQUEST['ntmqtdmatriculacrecheparcialpublica'] ? $_REQUEST['ntmqtdmatriculacrecheparcialpublica'] : '0';
		$ntmqtdmatriculacrecheintegralpublica		= $_REQUEST['ntmqtdmatriculacrecheintegralpublica'] ? $_REQUEST['ntmqtdmatriculacrecheintegralpublica'] : '0';
		$ntmqtdmatriculapreescolaparcialpublica		= $_REQUEST['ntmqtdmatriculapreescolaparcialpublica'] ? $_REQUEST['ntmqtdmatriculapreescolaparcialpublica'] : '0';
		$ntmqtdmatriculapreescolaintegralpublica	= $_REQUEST['ntmqtdmatriculapreescolaintegralpublica'] ? $_REQUEST['ntmqtdmatriculapreescolaintegralpublica'] : '0';
		$ntmqtdmatriculacrecheparcialconveniada		= $_REQUEST['ntmqtdmatriculacrecheparcialconveniada'] ? $_REQUEST['ntmqtdmatriculacrecheparcialconveniada'] : '0';
		$ntmqtdmatriculacrecheintegralconveniada	= $_REQUEST['ntmqtdmatriculacrecheintegralconveniada'] ? $_REQUEST['ntmqtdmatriculacrecheintegralconveniada'] : '0';
		$ntmqtdmatriculapreescolaparcialconveniada	= $_REQUEST['ntmqtdmatriculapreescolaparcialconveniada'] ? $_REQUEST['ntmqtdmatriculapreescolaparcialconveniada'] : '0';
		$ntmqtdmatriculapreescolaintegralconveniada	= $_REQUEST['ntmqtdmatriculapreescolaintegralconveniada'] ? $_REQUEST['ntmqtdmatriculapreescolaintegralconveniada'] : '0';
				
		if($ntmid){
			$sql = "update proinfantil.novasturmasdadosmunicipios set
						ntmqtdmatriculacrecheparcialpublica 		= {$ntmqtdmatriculacrecheparcialpublica},
						ntmqtdmatriculacrecheintegralpublica 		= {$ntmqtdmatriculacrecheintegralpublica},
						ntmqtdmatriculapreescolaparcialpublica 		= {$ntmqtdmatriculapreescolaparcialpublica},
						ntmqtdmatriculapreescolaintegralpublica 	= {$ntmqtdmatriculapreescolaintegralpublica},
						ntmqtdmatriculacrecheparcialconveniada 		= {$ntmqtdmatriculacrecheparcialconveniada},
						ntmqtdmatriculacrecheintegralconveniada 	= {$ntmqtdmatriculacrecheintegralconveniada},
						ntmqtdmatriculapreescolaparcialconveniada 	= {$ntmqtdmatriculapreescolaparcialconveniada},
						ntmqtdmatriculapreescolaintegralconveniada 	= {$ntmqtdmatriculapreescolaintegralconveniada}
					where ntmid = {$ntmid} and muncod = '{$_SESSION['proinfantil']['muncod']}'";
		} else {
			$sql = "insert into proinfantil.novasturmasdadosmunicipios
						(muncod,ntmstatus,ntmano,ntmsemestre,ntmqtdmatriculacrecheparcialpublica,ntmqtdmatriculacrecheintegralpublica,
						ntmqtdmatriculapreescolaparcialpublica,ntmqtdmatriculapreescolaintegralpublica,ntmqtdmatriculacrecheparcialconveniada,
						ntmqtdmatriculacrecheintegralconveniada,ntmqtdmatriculapreescolaparcialconveniada,ntmqtdmatriculapreescolaintegralconveniada)
					values
						('{$muncod}', 'A', '{$ntmano}', '{$ntmsemestre}', {$ntmqtdmatriculacrecheparcialpublica}, {$ntmqtdmatriculacrecheintegralpublica},
						{$ntmqtdmatriculapreescolaparcialpublica}, {$ntmqtdmatriculapreescolaintegralpublica}, {$ntmqtdmatriculacrecheparcialconveniada},
						{$ntmqtdmatriculacrecheintegralconveniada}, {$ntmqtdmatriculapreescolaparcialconveniada}, {$ntmqtdmatriculapreescolaintegralconveniada});";
		}
					
		$db->executar($sql);
		$db->commit();
	}
	
	function salvaAtualTurmasNT(){
		global $db;
		
		$muncod 						= $_SESSION['proinfantil']['muncod'];
		$ntmano 						= $_REQUEST['ntmano'];
		$ntmid							= $_REQUEST['ntmid'];
		$ntmsemestre 					= $_REQUEST['ntmsemestre'] ? $_REQUEST['ntmsemestre'] : '';
		$ntmqtdturmacrechepublica		= $_REQUEST['ntmqtdturmacrechepublica'] ? $_REQUEST['ntmqtdturmacrechepublica'] : '0';
		$ntmqtdturmapreescolapublica	= $_REQUEST['ntmqtdturmapreescolapublica'] ? $_REQUEST['ntmqtdturmapreescolapublica'] : '0';
		$ntmqtdturmaunificadapublica	= $_REQUEST['ntmqtdturmaunificadapublica'] ? $_REQUEST['ntmqtdturmaunificadapublica'] : '0';
		$ntmqtdturmacrecheconveniada	= $_REQUEST['ntmqtdturmacrecheconveniada'] ? $_REQUEST['ntmqtdturmacrecheconveniada'] : '0';
		$ntmqtdturmapreescolaconveniada	= $_REQUEST['ntmqtdturmapreescolaconveniada'] ? $_REQUEST['ntmqtdturmapreescolaconveniada'] : '0';
		$ntmqtdturmaunificadaconveniada	= $_REQUEST['ntmqtdturmaunificadaconveniada'] ? $_REQUEST['ntmqtdturmaunificadaconveniada'] : '0';
		
		if($ntmid){
			$sql = "update proinfantil.novasturmasdadosmunicipios set
						ntmqtdturmacrechepublica 		= {$ntmqtdturmacrechepublica},
						ntmqtdturmapreescolapublica 	= {$ntmqtdturmapreescolapublica},
						ntmqtdturmaunificadapublica 	= {$ntmqtdturmaunificadapublica},
						ntmqtdturmacrecheconveniada 	= {$ntmqtdturmacrecheconveniada},
						ntmqtdturmapreescolaconveniada 	= {$ntmqtdturmapreescolaconveniada},
						ntmqtdturmaunificadaconveniada 	= {$ntmqtdturmaunificadaconveniada}
					where ntmid = {$ntmid} and muncod = '{$_SESSION['proinfantil']['muncod']}'";
		} else {
			$sql = "insert into proinfantil.novasturmasdadosmunicipios
						(muncod,ntmstatus,ntmano,ntmsemestre,ntmqtdturmacrechepublica,ntmqtdturmapreescolapublica,ntmqtdturmaunificadapublica,
						ntmqtdturmacrecheconveniada,ntmqtdturmapreescolaconveniada,ntmqtdturmaunificadaconveniada)
					values
						('{$muncod}', 'A', '{$ntmano}', '{$ntmsemestre}', {$ntmqtdturmacrechepublica}, {$ntmqtdturmapreescolapublica},
						{$ntmqtdturmaunificadapublica}, {$ntmqtdturmacrecheconveniada}, {$ntmqtdturmapreescolaconveniada},
						{$ntmqtdturmaunificadaconveniada});";
		}
			
		$db->executar($sql);
		$db->commit();
	}
	
	function salvaDefault()	{
		if( $_POST['identExterno'] ){
			if( $_POST['queid'] == QUESTIONARIOPROMUN || $_POST['queid'] == QUESTIONARIOPROEST ){
		
				if( $_POST['perid'] == PERGUNTA_1 || $_POST['perid'] == PERGUNTA_2 ){
					global $db;
					$v = 0;
					if( $_POST ){
		
						$sql = "SELECT qptid FROM par.questionarioprofuncionariotutor WHERE qrpid = ".$_POST['qrpid'];
						if( $db->pegaUm( $sql ) ){
							$sql = "DELETE FROM par.questionarioprofuncionariotutor WHERE qrpid = ".$_POST['qrpid'];
							$db->executar( $sql );
						}
		
						if($_POST['cpf']){
							foreach( $_POST['cpf'] as $chave => $valor ){
								if( $v == 0 ){
									if( $valor && $_POST['nome'][$chave] && $_POST['email'][$chave] && $_POST['ano'][$chave] && $_POST['emexercicio'][$chave] ){
										$sql = "INSERT INTO par.questionarioprofuncionariotutor ( qptcpf, qptnome, qptemail, qptano, qptexerc, qrpid, qptqnt ) VALUES ( '{$valor}', '{$_POST['nome'][$chave]}', '{$_POST['email'][$chave]}', '{$_POST['ano'][$chave]}', '{$_POST['emexercicio'][$chave]}', {$_POST['qrpid']}, {$_POST['quant']} )";
										$db->carregar( $sql );
									} else {
										echo "<script>alert('Faltam dados');</script>";
										$v = 1;
									}
								}
							}
						}
						else{
							echo "<script>alert('Informe o(s) CPF(s)!');</script>";
							$v = 1;
						}
		
						if( $v == 0 ){
							$db->commit();
						}
					}
				}
			} else {
				foreach( $_POST['peridExterno'] as $perid ){
					$obQuest = new QuestoesPontuaisAnexos( );
		
					$obQuest->verifica( $_POST['qrpid'], $perid, $_POST['inuid'][$perid] );
		
					$campos	= array("qrpid"	        => $_POST['qrpid'],
							"perid"			=> $perid);
		
					$file = new FilesSimec("questoespontuaisanexos", $campos);
					if(is_file($_FILES["arquivo_".$perid]["tmp_name"])){
						$arquivoSalvo = $file->setUpload($_FILES["arquivo_".$perid]["name"], "arquivo_".$perid);
					}
				}
			}
		
		} else {
			$obIdent = new Identificacao( );
			$obIdent->excluirPorQuestionarioFormulario( $_POST['qrpid'], $_POST['perid'] );
			$obIdent->commit();
		
			$obIdent = new Identificacao( );
		
			$_REQUEST['idtdtpreench'] = formata_data_sql($_REQUEST['idtdtpreench']);
		
			$arrCampos = array('perid', 'qrpid', 'idtmun', 'idtuf', 'idtdtpreench', 'idtresp', 'idtcargo', 'idtformacao', 'idtemailorg',
					'idttelorg', 'idtnomeorg', 'idtend', 'idtresppreench', 'idttel', 'idtemail', 'idtger');
			$obIdent->popularObjeto( $arrCampos );
			$obIdent->idtmun = trim($_POST['idtmun']);
			$obIdent->salvar();
			$obIdent->commit();
		}
	}
}