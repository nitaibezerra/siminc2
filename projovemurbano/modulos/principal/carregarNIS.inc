<?php
// dl('ZipArchive');
// phpinfo();
// die;

// ini_set('memory_limit','4096M');
// ini_set('max_execution_time', 3000);
$perfis = pegaPerfilGeral();

include_once 'config.inc';

/**
 * Classe para acesso ao webservice de pessoa fÃ­sica.
 *
 * PS: NÃ£o esqueÃ§a de ler o leiame.txt
 *
 */
final class PessoaFisicaClient
{
	/**
	 * Coloca o objeto do cliente do webservice.
	 *
	 * @var SoapClient
	 */
	private $soapClient;


	/**
	 * Construtor da classe.
	 *
	 * @param string $wsdl
	 */
	public function __construct($wsdl)
	{

		try{
			$this->soapClient = new Simec_SoapClient( $wsdl );
		} catch (Exception $e){
			exit("NÃ£o estÃ¡ conectado!");
		}

	}

	/**
	 * Cliente Ws de Pessoa fÃ­sica da Receita Federal do Brasil.
	 *
	 * @param integer $cpf
	 * @return xml
	 */
	public function solicitarDadosPessoaFisicaPorCpf( $cpf )
	{
		return (  $this->soapClient->solicitarDadosPessoaFisicaPorCpf($cpf) );
	}

	/**
	 * Cliente Ws de Pessoa fÃ­sica da Receita Federal do Brasil.
	 *
	 * @param integer $cpf
	 * @return xml
	 */
	public function solicitarDadosResumidoPessoaFisicaPorCpf( $cpf )
	{
		return (  $this->soapClient->solicitarDadosResumidoPessoaFisicaPorCpf( $cpf ) );
	}

	/**
	 * Cliente Ws de Pessoa fÃ­sica da Receita Federal do Brasil.
	 *
	 * @param integer $cpf
	 * @return xml
	 */
	public static function solicitarDadosEnderecoPessoaFisicaPorCpf( $cpf )
	{
		return (  $this->soapClient->solicitarDadosEnderecoPessoaFisicaPorCpf( $cpf ) );
	}

	/**
	 * Cliente Ws de Pessoa fÃ­sica da Receita Federal do Brasil.
	 *
	 * @param integer $cpf
	 * @return xml
	 */
	public static function solicitarDadosContatoPessoaFisicaPorCpf( $cpf )
	{
		return (  $this->soapClient->solicitarDadosContatoPessoaFisicaPorCpf( $cpf ) );
	}
}

/**
 *
 */

function atualizarReceita($dados){
	global $db;
	
	$cps = explode(";",$dados['cpf']);
	$cpfs = $cps;
	
	$objPessoaFisica = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
	
	foreach($cpfs as $cpf){
		
		$cpf = str_replace(array('/', '.', '-'),array(''),str_replace(array(' '),'', $cpf));
		
		$sqltestaprojeto = "SELECT
							true
						FROM
							projovemurbano.cadastroestudante
						WHERE
							caecpf = '$cpf'";
		
		$testaprojeto = $db->pegaUm($sqltestaprojeto);
		if($testaprojeto == true){
			/**
			 * Aqui é feita a chamada do método da classe cliente do webservice.
			*/
			
			$xml 			 = $objPessoaFisica->solicitarDadosPessoaFisicaPorCpf($cpf);
			$obj = (array) simplexml_load_string($xml);
			
	// 		if (!$obj['PESSOA']) {
	// 			die();
	// 		}
			
			$pessoa   =  (array)$obj['PESSOA'];
			
			$sql .= "UPDATE projovemurbano.cadastroestudante
					 SET
						caenome = '{$pessoa['no_pessoa_rf']}',
						caedatanasc = '".trim(substr($pessoa['dt_nascimento_rf'],6,2)).'-'.trim(substr($pessoa['dt_nascimento_rf'],4,2)).'-'.trim(substr($pessoa['dt_nascimento_rf'],0,4))."',
						caenomemae = '{$pessoa['no_mae_rf']}'
					WHERE
						caecpf = '{$cpf}';
				";
			
		}
	}
// 	ver($sql,d);
	$db->executar($sql);
	$db->commit ();
	echo "<script>
			alert('Dados Atualizados com sucesso!');
		</script>";

}

function deletarAluno($dados){
	global $db;
	$sql = "DELETE CASCADE FROM projovemurbano.cadastroestudante
 			WHERE caecpf = '{$dados['caecpf']}';";
	$db->executar($sql);
	$db->commit ();
	echo "<script>
			alert('Aluno deletado com sucesso!');
		</script>";
	
}
function atualizapresenca($dados){
	global $db;
	$sql = "SELECT
				diaid,
				dia.turid,
				nucid
			FROM
				projovemurbano.diario dia
			INNER JOIN projovemurbano.turma tur on tur.turid = dia.turid
			WHERE
				perid = {$dados['perid1']}
			ORDER by diaid";
	
	$diarios = $db->carregar($sql);
	
	foreach($diarios as $diario){
				
		$sql = "SELECT 
	                    *
			  FROM projovemurbano.componentecurricular a
	                LEFT OUTER JOIN projovemurbano.gradecurricular b
	                    ON a.cocid = b.cocid
			LEFT OUTER JOIN projovemurbano.diariofrequencia c
	                    ON b.grdid 	= c.grdid
	                   AND c.diaid = {$diario['diaid']}
			 WHERE 
			 	a.cocstatus 	= 'A' 
	         AND a.cocdisciplina = 'D'
			  AND c.difqtdauladada is not null";
		$componentesCurriculares = $db->carregar($sql);
		
		foreach ($componentesCurriculares as $componenteCurricular){
			
			$sql = "SELECT 
						a.caeid, 
						a.caenome, 
						a.caestatus,
						frq.frqid, 
						frq.frqqtdpresenca 
					FROM projovemurbano.cadastroestudante a
					INNER JOIN projovemurbano.frequenciaestudante frq on frq.caeid = a.caeid
					INNER JOIN projovemurbano.diariofrequencia dif ON frq.difid = dif.difid
					INNER JOIN projovemurbano.gradecurricular grd ON dif.grdid = grd.grdid
					INNER JOIN projovemurbano.componentecurricular coc ON grd.cocid = coc.cocid
					WHERE 
						a.turid = {$diario['turid']}
					AND frq.difid = {$componenteCurricular['difid']}
					AND frq.frqqtdpresenca > {$componenteCurricular['difqtdauladada']}
					ORDER BY a.caenome";
			$dadosalunos = $db->carregar( $sql );
// 	if($dadosalunos){
// 		ver($componenteCurricular,$diario,$aluno,$sql,d);
// 	}
			if($dadosalunos){
				foreach ($dadosalunos as $aluno){
					
					$sql = "UPDATE projovemurbano.frequenciaestudante
							   SET frqqtdpresenca= {$componenteCurricular['difqtdauladada']}
							 WHERE frqid = {$aluno['frqid']} AND caeid = {$aluno['caeid']}";
					
					$db->executar ( $sql );
					$db->commit ();
				}
			}	
		}
	}
	echo "<script>
		alert('Endereços atualizados com sucesso!');
		</script>";
	
}

function atualizarEndereco(){

	global $db;

	$sqlaluno = "
					SELECT
						*
					FROM
							projovemurbano.cadastroestudante
					WHERE
						ppuid = 2
					AND caenispispasep is null or caenispispasep = ''
			 ";
	$caeid= $db->carregar($sqlaluno);
	
	foreach($caeid as $dados){


		$sqlendereco = "SELECT 
							*
						FROM
							projovemurbano.identificacaosecretario
						WHERE
							 pjuid = '{$dados['pjuid']}'
				 		AND  isestatus = 'A'";
		$endereco= $db->pegaLinha($sqlendereco);
		$sql = "UPDATE projovemurbano.cadastroestudante
  				SET 
				       caecep='{$endereco['isecep']}', 
				       caelogradouro='{$endereco['iseendereco']}', 
				       caebairro='{$endereco['isebairro']}',
				       estuf='{$endereco['iseuf']}', 
				       muncod='{$endereco['isemunicipio']}',
				       caenumero = '{$endereco['isenumero']}'
				 WHERE
						caeid = {$dados['caeid']};";
		$db->executar ( $sql );
		$db->commit();
	}
		echo "<script>
		alert('Endereços atualizados com sucesso!');
		</script>";

}
function atualizarBolsas(){
	
	global $db;
	
	$sqldiarios = "
					SELECT
						diaid
					FROM 
							projovemurbano.diario d
					INNER JOIN 	projovemurbano.periodocurso p ON p.perid = d.perid
					WHERE  
						p.ppuid = 2";
	$diarios= $db->carregarColuna($sqldiarios);
	
	foreach($diarios as $diaid){
		
		$sqlcontadora = "
						SELECT DISTINCT 
							(			
							SELECT DISTINCT
								count(doc1.docid)
							FROM projovemurbano.pagamentoestudante pge
							INNER JOIN workflow.documento doc1               		ON pge.docid = doc1.docid 
							INNER JOIN projovemurbano.cadastroestudante cae  		ON pge.caeid = cae.caeid
							INNER JOIN projovemurbano.diario dia             		ON pge.diaid = dia.diaid 
							INNER JOIN workflow.documento doc3               		ON dia.docid = doc3.docid
							INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
							INNER JOIN projovemurbano.turma tur              		ON dia.turid = tur.turid
							INNER JOIN projovemurbano.nucleo nuc             		ON nuc.nucid = tur.nucid 
							INNER JOIN projovemurbano.nucleoagenciabancaria nab     ON tur.nucid = nab.nucid
							INNER JOIN projovemurbano.nucleoescola nue       		ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
							INNER JOIN entidade.entidade entnuc              		ON entnuc.entid = nue.entid 
							INNER JOIN entidade.endereco endnuc              		ON endnuc.entid = entnuc.entid 
							INNER JOIN projovemurbano.projovemurbano pju     		ON pju.pjuid = cae.pjuid
							WHERE 
								doc1.esdid = 527	 
							AND dia.diaid = $diaid
							AND doc3.esdid IN(523,524)
							AND pge.pgeaptoreceber = 't' 
							AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
							)as pendentes,
							(
							SELECT DISTINCT
								count(doc1.docid)
							FROM projovemurbano.pagamentoestudante pge
							INNER JOIN workflow.documento doc1               		ON pge.docid = doc1.docid 
							INNER JOIN projovemurbano.cadastroestudante cae  		ON pge.caeid = cae.caeid
							INNER JOIN projovemurbano.diario dia             		ON pge.diaid = dia.diaid 
							INNER JOIN workflow.documento doc3               		ON dia.docid = doc3.docid
							INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
							INNER JOIN projovemurbano.turma tur              		ON dia.turid = tur.turid
							INNER JOIN projovemurbano.nucleo nuc             		ON nuc.nucid = tur.nucid 
							INNER JOIN projovemurbano.nucleoagenciabancaria nab     ON tur.nucid = nab.nucid
							INNER JOIN projovemurbano.nucleoescola nue       		ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
							INNER JOIN entidade.entidade entnuc              		ON entnuc.entid = nue.entid 
							INNER JOIN entidade.endereco endnuc              		ON endnuc.entid = entnuc.entid 
							INNER JOIN projovemurbano.projovemurbano pju     		ON pju.pjuid = cae.pjuid
							WHERE 
								doc1.esdid = 528	 
							AND dia.diaid = $diaid			
							AND doc3.esdid IN(523,524)
							AND pge.pgeaptoreceber = 't' 
							AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
							)as autorizadas,
							(
							SELECT DISTINCT
								count(doc1.docid)
							FROM projovemurbano.pagamentoestudante pge
							INNER JOIN workflow.documento doc1               		ON pge.docid = doc1.docid 
							INNER JOIN projovemurbano.cadastroestudante cae  		ON pge.caeid = cae.caeid
							INNER JOIN projovemurbano.diario dia             		ON pge.diaid = dia.diaid 
							INNER JOIN workflow.documento doc3               		ON dia.docid = doc3.docid
							INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
							INNER JOIN projovemurbano.turma tur              		ON dia.turid = tur.turid
							INNER JOIN projovemurbano.nucleo nuc             		ON nuc.nucid = tur.nucid 
							INNER JOIN projovemurbano.nucleoagenciabancaria nab     ON tur.nucid = nab.nucid
							INNER JOIN projovemurbano.nucleoescola nue       		ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
							INNER JOIN entidade.entidade entnuc              		ON entnuc.entid = nue.entid 
							INNER JOIN entidade.endereco endnuc              		ON endnuc.entid = entnuc.entid 
							INNER JOIN projovemurbano.projovemurbano pju     		ON pju.pjuid = cae.pjuid
							WHERE 
								doc1.esdid = 529	 
							AND dia.diaid = $diaid
							AND doc3.esdid IN(523,524)
							AND pge.pgeaptoreceber = 't' 
							AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
							)as enviadas,
							(
							SELECT DISTINCT
								count(doc1.docid)
							FROM projovemurbano.pagamentoestudante pge
							INNER JOIN workflow.documento doc1               		ON pge.docid = doc1.docid 
							INNER JOIN projovemurbano.cadastroestudante cae  		ON pge.caeid = cae.caeid
							INNER JOIN projovemurbano.diario dia             		ON pge.diaid = dia.diaid 
							INNER JOIN workflow.documento doc3               		ON dia.docid = doc3.docid
							INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
							INNER JOIN projovemurbano.turma tur              		ON dia.turid = tur.turid
							INNER JOIN projovemurbano.nucleo nuc             		ON nuc.nucid = tur.nucid 
							INNER JOIN projovemurbano.nucleoagenciabancaria nab     ON tur.nucid = nab.nucid
							INNER JOIN projovemurbano.nucleoescola nue       		ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
							INNER JOIN entidade.entidade entnuc              		ON entnuc.entid = nue.entid 
							INNER JOIN entidade.endereco endnuc              		ON endnuc.entid = entnuc.entid 
							INNER JOIN projovemurbano.projovemurbano pju     		ON pju.pjuid = cae.pjuid
							WHERE 
								doc1.esdid = 560	 
							AND dia.diaid = $diaid
							AND doc3.esdid IN(523,524)
							AND pge.pgeaptoreceber = 't' 
							AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
							)as rejeitadas	
						FROM projovemurbano.pagamentoestudante pge
						INNER JOIN workflow.documento doc                ON pge.docid = doc.docid 
						INNER JOIN projovemurbano.cadastroestudante cae        ON pge.caeid = cae.caeid
						INNER JOIN projovemurbano.diario dia             ON pge.diaid = dia.diaid 
						INNER JOIN workflow.documento doc2               ON dia.docid = doc2.docid
						INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
						INNER JOIN projovemurbano.turma tur              ON dia.turid = tur.turid
						INNER JOIN projovemurbano.nucleo nuc             ON nuc.nucid = tur.nucid 
						INNER JOIN projovemurbano.nucleoagenciabancaria nab    ON tur.nucid = nab.nucid
						INNER JOIN projovemurbano.nucleoescola nue       ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
						INNER JOIN entidade.entidade entnuc              ON entnuc.entid = nue.entid 
						INNER JOIN entidade.endereco endnuc              ON endnuc.entid = entnuc.entid 
						INNER JOIN projovemurbano.projovemurbano pju     ON pju.pjuid = cae.pjuid
						WHERE 
							pge.pgeaptoreceber = 't' 
						AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0 
						AND dia.diaid = $diaid
						AND doc2.esdid IN(523,524)
						";
		$contados = $db->pegaLinha($sqlcontadora);
		if($contados){
			$sqlinsertqtds = "UPDATE projovemurbano.diario
								SET diabolsaspendentes = {$contados['pendentes']}, diabolsasautorizadas = {$contados['autorizadas']}, diabolsasenviadas = {$contados['enviadas']}, diabolsasrecusadas = {$contados['rejeitadas']}
								WHERE 
									diaid =$diaid";
// 		ver($sqlinsertqtds,d);
			$db->executar($sqlinsertqtds);
			$db->commit();
		}
	}
	echo "<script>
			alert('Dados atualizados com sucesso!');
	    </script>";
	
}


function desfazertransferencia($dados){
	
	global $db;
	/*Carregas os dados do aluno referente a turma que ele estava antes de ser transferido */
	$sqlcarregadados = "SELECT 
							pjuid_origem,
							polid_origem,
							nucid_origem,
							turid_origem,
							ppuid_origem
						FROM
							projovemurbano.transferencia
						WHERE
							traid = (SELECT
									MIN(tra.traid)
								 FROM
									projovemurbano.transferencia tra
								 LEFT JOIN projovemurbano.historico_transferencia hst ON tra.traid = hst.traid AND shtid_status = 3
								 WHERE
									tra.cad_caeid = {$dados['caeid']})
						AND cad_caeid = {$dados['caeid']}";
// 	ver($sqlcarregadados,d);
	$carregadados = $db->pegaLinha($sqlcarregadados);
	
	$carregadados['polid_origem'] = $carregadados['polid_origem'] ?"'".$carregadados['polid_origem']."'": 'NULL';
	
	/*Fim*/
/*Volta os dados do aluno para o que eram antes*/	
	/*Faz um update na tabela cadastroestudante com os dados que ele tinha antes da 1° transferência.*/
	$sqlalteradados = "	UPDATE projovemurbano.cadastroestudante
					   	SET pjuid = {$carregadados['pjuid_origem']},
							polid = {$carregadados['polid_origem']}, 
							nucid = {$carregadados['nucid_origem']}, 
							turid = {$carregadados['turid_origem']},
							ppuid = {$carregadados['ppuid_origem']} 
						WHERE
							caeid = {$dados['caeid']}
	";
	$db->executar($sqlalteradados);
	/*FIM*/
	/*carrega o primeoro registro de nota e faz update com os dados que ele tinha antes da 1° transferência*/
	$sqlprimeironpcid = " SELECT
							MIN(npcid)
						FROM 
							projovemurbano.notasporciclo
						WHERE
						    caeid = {$dados['caeid']}";
	$primeironpcid = $db->pegaUm($sqlprimeironpcid);
	
	$sqlalteranotas = "	UPDATE projovemurbano.notasporciclo
					   	SET npc_status = 'A'
						WHERE
							npcid = {$primeironpcid}
						AND caeid = {$dados['caeid']}";
	$db->executar($sqlalteranotas);
	/*FIM*/
	/*Faz um  na tabela de frequencia com os dados que ele tinha antes da 1° transferência*/
	$sqlalterafrequencia = "UPDATE projovemurbano.frequenciaestudante
							SET frqstatus = 'A'
							WHERE
									caeid = {$dados['caeid']}
							AND     difid in(	SELECT
													dif2.difid
												FROm
													projovemurbano.frequenciaestudante frq2 
												INNER JOIN projovemurbano.diariofrequencia dif2 ON dif2.difid = frq2.difid
												INNER JOIN projovemurbano.diario dia2 ON dia2.diaid = dif2.diaid  
												WHERE
														caeid = {$dados['caeid']}
												AND     turid = {$carregadados['turid_origem']})";
	$db->executar($sqlalterafrequencia);
	/*FIM*/
/*Fim Volta dados*/
/*Deleta dados da transferencia*/
	/*Deleta os registros gerados pela transferência na tabela de notas*/	
	$sqldeletanotas = "	DELETE FROM projovemurbano.notasporciclo
						WHERE
						npcid != {$primeironpcid}
						AND caeid = {$dados['caeid']}";
	
	$db->executar($sqldeletanotas);
	/*FIM*/
	/*Deleta os registros gerados pela transferência na tabela de frequencias*/
	$sqldeletafrequencia = "DELETE FROM projovemurbano.frequenciaestudante
							WHERE
							caeid = {$dados['caeid']}
							AND     difid in(	SELECT
													dif2.difid
												FROm
												projovemurbano.frequenciaestudante frq2
												INNER JOIN projovemurbano.diariofrequencia dif2 ON dif2.difid = frq2.difid
												INNER JOIN projovemurbano.diario dia2 ON dia2.diaid = dif2.diaid
												WHERE
														caeid = {$dados['caeid']}
												AND     turid != {$carregadados['turid_origem']})";
	$db->executar($sqldeletafrequencia);
	/*FIM*/
	$sqlhstid= "
				SELECT
					htrid
			    FROM
					projovemurbano.historico_transferencia
				WHERE
					  traid in (SELECT
									traid
							    FROM
									projovemurbano.transferencia
							    WHERE
									cad_caeid = {$dados['caeid']})";
	$htrid= $db->carregarColuna($sqlhstid);
	$htrid = implode(',',$htrid);
	
	$sqluptra = "UPDATE projovemurbano.transferencia
				 SET htrid_ultimo = NULL
				 WHERE
					cad_caeid = {$dados['caeid']}";
	$db->executar($sqluptra);
	
	/*Deleta os registros gerados pela transferência na tabela de histórico de transferência*/
	$sqluphst = "DELETE FROM projovemurbano.historico_transferencia
					WHERE
					htrid in ($htrid)";
	$db->executar($sqluphst);
	
	$sqldeletatabelatra = " DELETE FROM projovemurbano.transferencia
							WHERE
								cad_caeid = {$dados['caeid']}";
	$db->executar($sqldeletatabelatra);
	
	/*FIM*/
	
	/*Deleta os registros gerados pela transferência na tabela de transferência*/
	
	
	$db->commit();
	
	/*FIM*/
/*Fim deleta dados da transferência*/	
	echo "<script>
			alert('Todos os dados relativos a transferência desse aluno foram deletados. Agora ele se encontra com se nunca houvesse sido tranferido.');
	    </script>";
}

function atualizatransferencia(){
	
	global $db;
	
	$sqlerrostras = "SELECT DISTINCT
						tra.cad_caeid as caeid
					FROM
						projovemurbano.transferencia tra
					INNER JOIN projovemurbano.cadastroestudante cae ON cae.caeid = tra.cad_caeid
					INNER JOIN projovemurbano.historico_transferencia hst ON hst.traid = tra.traid AND shtid_status = 3
					ORDER BY
						caeid
					";
	
	$errostras = $db->carregarColuna($sqlerrostras);
// 	ver($errostras,d);
	foreach($errostras as $alunoerro){
		
		$pjuidcerto = NULL;
		$dadostransferencia['turid_destino'] = NULL;
		$dadostransferencia['nucid_destino'] = NULL;
		$dadostransferencia['polid_destino'] = NULL;
		$dadostransferencia['ppuid_destino'] = NULL;
		$dadostransferencia['turid'] = NULL;
		$dadostransferencia['nucid'] = NULL;
		$dadostransferencia['polid'] = NULL;
		$dadostransferencia['pjuid'] = NULL;
		
		$sqldadostransferencia = " SELECT 
										tra.turid_destino,
										tra.nucid_destino,
										tra.polid_destino,
										cae.turid,
										cae.nucid,
										cae.polid,
										tra.ppuid_destino,
										cae.pjuid
									FROM
										projovemurbano.transferencia tra
									INNER JOIN projovemurbano.cadastroestudante cae ON cae.caeid = tra.cad_caeid
									INNER JOIN projovemurbano.historico_transferencia hst ON hst.traid = tra.traid AND shtid_status = 3
									WHERE
									     tra.cad_caeid = $alunoerro
								    AND  tra.traid = (	SELECT 
															MAX(tra1.traid) 
													    FROM
															projovemurbano.transferencia tra1
														INNER JOIN projovemurbano.cadastroestudante cae1 ON cae1.caeid = tra1.cad_caeid
														INNER JOIN projovemurbano.historico_transferencia hst1 ON hst1.traid = tra1.traid AND hst1.shtid_status = 3
														WHERE
															cae1.caeid = $alunoerro)
									GROUP BY
										tra.turid_destino,
										tra.nucid_destino,
										tra.polid_destino,
										cae.turid,
										cae.nucid,
										cae.polid,
										tra.ppuid_destino,
										cae.pjuid
									";
		
		$dadostransferencia = $db->pegaLinha($sqldadostransferencia);
		
		if($dadostransferencia['polid_destino'] != ''){
			/*Estadual com polo*/
			$sqlpjuidcerto = "  SELECT DISTINCT
					                pju.pjuid
								FROM projovemurbano.nucleo nuc
				                INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				                INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
				                INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
				                INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
				                INNER JOIN projovemurbano.projovemurbano pju ON plm.pjuid = pju.pjuid
								WHERE 
					                --nuc.nucstatus='A' 
				                --AND 
				                	mun.munstatus='A' 
				                AND plm.pmustatus='A' 
						        AND pol.polstatus='A'  
						        AND nuc.ppuid = {$dadostransferencia['ppuid_destino']} 
						        AND pju.estuf <>''
		 						AND pol.polid = {$dadostransferencia['polid_destino']}
						        ";
			$pjuidcerto = $db->pegaUM($sqlpjuidcerto);
			
			if(!$pjuidcerto){
			/*Municipal com polo*/	
			$sqlpjuidcerto = " SELECT DISTINCT
					                pju.pjuid
								FROM projovemurbano.nucleo nuc
				                INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				                INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
				                INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
				                INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
				                INNER JOIN projovemurbano.projovemurbano pju ON plm.pjuid = pju.pjuid
								WHERE 
					                --nuc.nucstatus='A' 
				                --AND 
				                	mun.munstatus='A' 
				                AND plm.pmustatus='A' 
						        AND pol.polstatus='A' 
						        AND nuc.ppuid = {$dadostransferencia['ppuid_destino']} 
						        AND pju.muncod <> ''
								AND pol.polid = {$dadostransferencia['polid_destino']}
						";
			$pjuidcerto = $db->pegaUM($sqlpjuidcerto);
			}
			
		}ELSE{
			/*Estadual sem polo*/
			$sqlpjuidcerto = "  SELECT DISTINCT
					                pju.pjuid
								FROM 
					                projovemurbano.nucleo nuc 
						        INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
						        INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
						        INNER JOIN projovemurbano.projovemurbano pju ON plm.pjuid = pju.pjuid
								WHERE 
					                --nuc.nucstatus='A'
					                --AND 
					                	mun.munstatus='A' 
					                AND plm.pmustatus='A' 
							        AND nuc.ppuid = {$dadostransferencia['ppuid_destino']} 
							        AND pju.estuf <> ''
									AND nuc.nucid = {$dadostransferencia['nucid_destino']}
									";
			$pjuidcerto = $db->pegaUM($sqlpjuidcerto);
			/*Municpal sem polo*/
			if(!$pjuidcerto){
				$sqlpjuidcerto = "  SELECT DISTINCT
						                pju.pjuid
									FROM 
							                projovemurbano.nucleo nuc 
							        INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
							        INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
							        INNER JOIN projovemurbano.projovemurbano pju ON plm.pjuid = pju.pjuid
									WHERE 
						                --nuc.nucstatus='A' 
					                --AND 
					                	--mun.munstatus='A' 
					                --AND 
					                	plm.pmustatus='A' 
							        AND nuc.ppuid = {$dadostransferencia['ppuid_destino']} 
							        AND pju.muncod <> ''   
									AND nuc.nucid = {$dadostransferencia['nucid_destino']} 
									";
				$pjuidcerto = $db->pegaUM($sqlpjuidcerto);
			}
		}
		if(!$pjuidcerto){
			ver($sqlpjuidcerto,$dadostransferencia['polid_destino'],d);
		}
// 		if($dadostransferencia['turid_destino'] != $dadostransferencia['turid']){
			$turma = " ,"."turid =  {$dadostransferencia['turid_destino']}";
// 		}
		
// 		if($dadostransferencia['nucid_destino'] != $dadostransferencia['nucid']){
			$nucleo = " , "."nucid = {$dadostransferencia['nucid_destino']}";
// 	}
		
		
// 		if(($dadostransferencia['polid_destino'] != $dadostransferencia['polid'])||(!$dadostransferencia['polid_destino'])){
			$dadostransferencia['polid_destino'] = $dadostransferencia['polid_destino'] ? "'".$dadostransferencia['polid_destino']."'" : 'NULL';
			$polo = " , "."polid =  {$dadostransferencia['polid_destino']}";
// 		}
		
		
// 		if(($pjuidcerto != $dadostransferencia['pjuid'])
// 			||($dadostransferencia['polid_destino'] != $dadostransferencia['polid'])
// 			||$dadostransferencia['nucid_destino'] != $dadostransferencia['nucid']
// 			||$dadostransferencia['turid_destino'] != $dadostransferencia['turid']){
			$sql = "UPDATE projovemurbano.cadastroestudante
					   SET pjuid = $pjuidcerto, ppuid = {$dadostransferencia['ppuid_destino']} $turma $nucleo $polo
					WHERE
						caeid = $alunoerro;";
// 			ver($sql,d);
			$db->executar($sql);
			$db->commit();
// 		}
	}
	
	echo "<script>
			alert('Dados atualizados com sucesso!');
	    </script>";

}

include_once APPRAIZ . 'includes/workflow.php';

function atualizapagamento($dados){
	
	global $db;
	
	$sqlpuxadiario = "SELECT
							diaid
						FROM
							projovemurbano.diario
						WHERE
							perid = {$dados['perid']}";
	$puxadiario = $db->carregarColuna($sqlpuxadiario);
// 	ver($puxadiario,d);
	
	$atualizanis = "update projovemurbano.cadastroestudante set caenispispasep = null where caenispispasep = '0'";
	$db->executar($atualizanis);
	$db->commit();
	
	foreach($puxadiario as $diario){
		
		$sqlalunos = "SELECT
                  	foo.caeid as caeid
                  FROM
                  	(
					SELECT 
                    	cae.caeid, 
                        cae.caenome, 
                        cae.caenispispasep, 
                        nab.nabid, 
                        presenca_estudante.total_presenca_componente, 
                        trabalho_aluno.total_trabalho_entregue,
                        cae.caeqtddireitobolsa,
                        caeqtdbolsaprojovem
                    FROM projovemurbano.diario dia
                    INNER JOIN projovemurbano.turma tur ON dia.turid = tur.turid
                    INNER JOIN projovemurbano.cadastroestudante cae ON tur.turid = cae.turid
                    INNER JOIN projovemurbano.nucleo nuc ON tur.nucid = nuc.nucid
                    LEFT JOIN projovemurbano.nucleoagenciabancaria nab ON nuc.nucid = nab.nucid
                    LEFT JOIN(
                              SELECT cae.caeid, 
									 cae.caenome, 
									 COALESCE(SUM( frq.frqqtdpresenca ), 0 )  as total_presenca_componente
                              from projovemurbano.diario dia             
                              INNER JOIN projovemurbano.diariofrequencia dif ON dia.diaid = dif.diaid AND dia.diaid = $diario
                              INNER JOIN projovemurbano.cadastroestudante cae ON dia.turid =  cae.turid AND cae.caestatus = 'A'
                              INNER JOIN projovemurbano.gradecurricular grd ON grd.grdid = dif.grdid
                              INNER JOIN projovemurbano.componentecurricular coc ON coc.cocid = grd.cocid AND coc.cocdisciplina = 'D'
                              LEFT JOIN projovemurbano.frequenciaestudante frq ON dif.difid    = frq.difid AND cae.caeid = frq.caeid
                              GROUP BY cae.caeid,cae.caenome
                              ORDER BY cae.caenome
                             ) as presenca_estudante ON cae.caeid = presenca_estudante.caeid
                    LEFT JOIN(
                              SELECT 
								cae.caeid, 
								cae.caenome, 
								COUNT ( frq.frqid ) as total_trabalho_entregue, 
								COUNT ( frq2.frqid ) as total_trabalho_nao_entregue
                              FROM projovemurbano.diario dia
                              INNER JOIN projovemurbano.diariofrequencia dif ON dia.diaid = dif.diaid AND dia.diaid = $diario               
                              INNER JOIN projovemurbano.gradecurricular grd ON dif.grdid = grd.grdid
                              INNER JOIN projovemurbano.componentecurricular coc ON grd.cocid = coc.cocid AND coc.cocdisciplina = 'T'
                              INNER JOIN projovemurbano.cadastroestudante cae ON dia.turid = cae.turid AND cae.caestatus = 'A'
                              LEFT JOIN projovemurbano.frequenciaestudante frq ON dif.difid = frq.difid AND cae.caeid = frq.caeid AND frq.frqtrabalho = 't' AND frq.frqstatus = 'A'
                              LEFT JOIN projovemurbano.frequenciaestudante frq2 ON dif.difid = frq2.difid AND cae.caeid = frq2.caeid  AND frq2.frqtrabalho = 'f' AND frq2.frqstatus = 'A'           
                              GROUP BY cae.caeid, cae.caenome 
							 ) as trabalho_aluno ON cae.caeid = trabalho_aluno.caeid
                  WHERE dia.diaid = $diario
                  AND presenca_estudante.total_presenca_componente >= (0.75*(SELECT 
                                                                             SUM(difqtdauladada) AS soma_difqtdauladada
                                                                             FROM projovemurbano.diariofrequencia dif
                                                                             INNER JOIN projovemurbano.gradecurricular grd ON dif.grdid = grd.grdid
                                                                             INNER JOIN projovemurbano.componentecurricular coc ON grd.cocid = coc.cocid
                                                                             WHERE diaid = $diario))
                  AND trabalho_aluno.total_trabalho_entregue >= 3
                  
                  AND nab.nabid is not null
                  AND nab.nabid > 0
                  AND (cae.caeqtddireitobolsa - caeqtdbolsaprojovem) >= 0
                ORDER BY caenome) as foo";
		/*AND cae.caenispispasep is not null
                  AND cae.caenispispasep::numeric != 0
                  AND cae.caenispispasep != ''*/
// 		ver($sqlalunos,d);
		$caeids = $db->carregarColuna($sqlalunos);
		foreach ($caeids as $caeid){
			
			$sqlVerificaPagamento = "SELECT pgeid FROM projovemurbano.pagamentoestudante
									WHERE caeid = {$caeid} AND diaid = {$diario}";
			$VerificaPagamento = $db->pegaUm( $sqlVerificaPagamento);
			
			$docId = wf_cadastrarDocumento( WORKFLOW_TIPODOCUMENTO_PAGAMENTO, 'Fluxo do pagamento de estudante' );
// 				ver($VerificaPagamento,d);
			if($VerificaPagamento==''){
				$sqlInserePagamento     = "INSERT INTO projovemurbano.pagamentoestudante( caeid, diaid, pgeaptoreceber, docid )
											VALUES( {$caeid}, {$diario}, 't', {$docId}) RETURNING pgeid";
				$db->pegaUm($sqlInserePagamento);
			
				$sqlAtualizaAptoAReceber = "SELECT projovemurbano.fn_situacao_aluno_pagto({$caeid}, {$diario});";
				$db->pegaUm( $sqlAtualizaAptoAReceber );
			}
		}
		$db->commit();
		if($caeids['0'] != ''){
			$atualizapagamento = "UPDATE projovemurbano.pagamentoestudante
									   SET pgeaptoreceber='t'
									WHERE diaid = $diario
									--AND  caeid = $caeid
									AND caeid IN (".implode(',', $caeids).")";
			
			$db->executar($atualizapagamento);
			$db->commit();
		}
	}
	echo "<script>
			alert('Dados atualizados');
	    </script>";
}

function executarCarga($dados) {
	global $db;
	
	ini_set("memory_limit", "2048M");
	set_time_limit(800000);
	ini_set('max_execution_time',300000);
// 	ini_set('post_max_size', '128M');
// 	ini_set('upload_max_filesize', '128M');
	
// 	if ($_FILES['arquivo']['name']) {
// 		if(!is_dir('../../arquivos/projovemurbano/nis')) {
// 			mkdir(APPRAIZ.'/arquivos/projovemurbano/nis', 0777);
// 		}
		
		
		$pathDir = APPRAIZ . 'arquivos/projovemurbano/nis';
		
		$aArquivos = glob($pathDir."/*");
		
		foreach($aArquivos as $arquivo){
			unlink($arquivo);
		}		
		
		
		$targetFile = $pathDir . '/' . $_FILES['arquivo']['name'];
	
		move_uploaded_file($_FILES['arquivo']['tmp_name'], $targetFile);
	
		$aTiposCompactados = array('application/zip', 'application/x-msdownload', 'application/x-zip-compressed');
		
		// Se for ZIP (Única extensão aceita)

		if(in_array($_FILES['arquivo']['type'], $aTiposCompactados)){
			$zip = new ZipArchive;
	
			// Extrai arquivo zip
			$retorno = $zip->open($targetFile);
			
			if($retorno === true){
				
				$zip->extractTo($pathDir.'/');
				$zip->close();
	
				// Apagando arquivo zip
				unlink($targetFile);
			}
		}
		// DÃ¡ um loop nos arquivos da pasta
		$aArquivos = glob($pathDir."/*");
		
		foreach($aArquivos as $arquivo){
			
// 			$file = fopen($arquivo, 'r');
			
			$arrCSV = Array();
			$y = 0;
			
// 			while ($csvarray = file($arquivo)) {
				$csvarray = file($arquivo);
// 				phpinfo();
				foreach( $csvarray as $k => $linha){
// 					if( ($k-14)%18 == 0 ){
					
						$linha = array_unique(explode(' ', $linha));
						$yy = 0;
						foreach( $linha as $celula){
// 							ver($celula);
							if( strlen(trim($celula)) > 15 ){
								$arrCSV[$y][$yy] = substr ( trim($celula), 53, 11);
								$yy++;
								$arrCSV[$y][$yy] = substr ( trim($celula),45,4);
								$yy++;
							}elseif(strlen(trim($celula)) == 15){
								$arrCSV[$y][$yy] = trim($celula);
								$yy++;
							}
						}
						$y++;
// 					}
				}
// 			}
				
			foreach( $arrCSV as  $dadosASalvar ){
				
				$checanis = '';

				$nis = (substr($dadosASalvar['2'],0,11));
				
// 				if($dadosASalvar['0']=='09074343473'){
// 					ver($nis,d);
// 				}		
				if($nis == '' || $dadosASalvar['1'] != '0370'){
					continue;
				}else{
					$sqlchecanis = "SELECT 
										caenispispasep 
									FROM 
										projovemurbano.cadastroestudante 
									WHERE 
										caecpf = '{$dadosASalvar['0']}'";
					$checanis = $db->pegaUm( $sqlchecanis );
					
					if(($checanis == '' )|| ($checanis == NULL)||($checanis == '           ')||($checanis == 0)){
						
						$sqlupdate = "
								UPDATE projovemurbano.cadastroestudante
								SET caenispispasep= $nis
								WHERE caecpf = '{$dadosASalvar['0']}';
								";
						
						$db->executar($sqlupdate );
						$db->commit();
						
					}
				}
			}
		}
		echo "<script>
			alert('Arquivos NIS Carregados.');
	    </script>";			
	}	
	/*********/
	
// 	function verificaAlunoApto() {
// 		global $db;
// 		$diaid = 54039;
// 		$sqlTotalAulasDadas = "SELECT SUM(difqtdauladada) AS soma_difqtdauladada
// 		FROM projovemurbano.diariofrequencia dif
// 		INNER JOIN projovemurbano.gradecurricular grd
// 		ON dif.grdid = grd.grdid
// 		INNER JOIN projovemurbano.componentecurricular coc
// 		ON grd.cocid = coc.cocid
// 		WHERE diaid = {$diaid}";
	
// 		$sqlAluno = "SELECT cae.caeid, cae.caenome, cae.caenispispasep
// 		, nab.nabid, presenca_estudante.total_presenca_componente
// 		, trabalho_aluno.total_trabalho_entregue
// 		, cae.caeqtddireitobolsa
// 		,caeqtdbolsaprojovem
// 		FROM projovemurbano.diario dia
// 		INNER JOIN projovemurbano.turma tur
// 		ON dia.turid = tur.turid
// 		INNER JOIN projovemurbano.cadastroestudante cae
// 		ON tur.turid = cae.turid
// 		INNER JOIN projovemurbano.nucleo nuc
// 		ON tur.nucid = nuc.nucid
// 		LEFT JOIN projovemurbano.nucleoagenciabancaria nab
// 		ON nuc.nucid = nab.nucid
// 		LEFT JOIN
// 		(
// 		SELECT cae.caeid, cae.caenome
// 		, COALESCE(SUM( frq.frqqtdpresenca ), 0 )  as total_presenca_componente
// 		from projovemurbano.diario dia
// 		INNER JOIN projovemurbano.diariofrequencia dif
// 		ON dia.diaid = dif.diaid AND dia.diaid = {$diaid}
// 		INNER JOIN projovemurbano.cadastroestudante cae
// 		ON dia.turid =  cae.turid
// 		AND cae.caestatus = 'A'
// 		INNER JOIN projovemurbano.gradecurricular grd
// 		ON grd.grdid = dif.grdid
// 		INNER JOIN projovemurbano.componentecurricular coc
// 		ON coc.cocid = grd.cocid AND coc.cocdisciplina = 'D'
// 		LEFT JOIN projovemurbano.frequenciaestudante frq
// 		ON dif.difid  	= frq.difid
// 		AND cae.caeid	= frq.caeid
// 		GROUP BY cae.caeid,cae.caenome
// 		ORDER BY cae.caenome
// 		) as presenca_estudante
// 		ON cae.caeid = presenca_estudante.caeid
	
	
// 		LEFT JOIN
// 		(
// 		SELECT cae.caeid, cae.caenome, COUNT ( frq.frqid ) as total_trabalho_entregue
// 		, COUNT ( frq2.frqid ) as total_trabalho_nao_entregue
// 		FROM projovemurbano.diario dia
// 		INNER JOIN projovemurbano.diariofrequencia dif
// 		ON dia.diaid = dif.diaid AND dia.diaid = {$diaid}
// 		INNER JOIN projovemurbano.gradecurricular grd
// 		ON dif.grdid = grd.grdid
// 		INNER JOIN projovemurbano.componentecurricular coc
// 		ON grd.cocid = coc.cocid AND coc.cocdisciplina = 'T'
// 		INNER JOIN projovemurbano.cadastroestudante cae
// 		ON dia.turid = cae.turid AND cae.caestatus	= 'A'
// 		LEFT JOIN projovemurbano.frequenciaestudante frq
// 		ON dif.difid 		= frq.difid
// 		AND cae.caeid 		= frq.caeid
// 		AND frq.frqtrabalho 	= 't'
// 		AND frq.frqstatus = 'A'
// 		LEFT JOIN projovemurbano.frequenciaestudante frq2
// 		ON dif.difid = frq2.difid
// 		AND cae.caeid = frq2.caeid
// 		AND frq2.frqtrabalho 	= 'f'
// 		AND frq2.frqstatus = 'A'
// 		GROUP BY cae.caeid, cae.caenome
// 		) as trabalho_aluno
// 		ON cae.caeid = trabalho_aluno.caeid
// 		WHERE dia.diaid = {$diaid}
// 		ORDER BY caenome";
// 		// ver($sql,d);
// 		$dadosAlunos = $db->carregar ( $sqlAluno );
// 		$totalAulasDadas = $db->pegaUm ( $sqlTotalAulasDadas );
	
// 		if ($totalAulasDadas == 0) {
// 		throw new Exception ( 'Não foi possível fechar o diário. Informe a quantidade de aulas dadas' );
// 	}
	
// 	foreach ( $dadosAlunos as $aluno ) {
	
// 	$apto = "t";
// 	$porcentagemPresenca = (($aluno ['total_presenca_componente'] * 100) / $totalAulasDadas);
	
// 	// aluno com menos de 3 trabalhos
// 	if ($aluno ['total_trabalho_entregue'] < 3) {
// 	$apto = "f";
// 	}
	
// 	// aluno sem agência ou nis
// 	if (empty ( $aluno ['nabid'] ) || empty ( $aluno ['caenispispasep'] ) || $aluno ['caenispispasep'] == 0) {
// 	$apto = "f";
// 	}
	
// 	// aluno sem 75% de presenca
// 	if ($porcentagemPresenca < 0.75) {
// 	$apto = "f";
// 	}
	
// 	// aluno com direito a receber a bolsa
// 	if ($aluno ['caeqtddireitobolsa'] - $aluno ['caeqtdbolsaprojovem'] <= 0) {
// 	$apto = "f";
// 	}
// // 	if($aluno['caeid'] == 202859){
// // 		ver($apto);
// // 	}
// 	$sqlVerificaPagamento = "SELECT * FROM projovemurbano.pagamentoestudante
// 	WHERE caeid = {$aluno['caeid']} AND diaid = {$diaid}";
// 	$VerificaPagamento = $db->pegaLinha ( $sqlVerificaPagamento );
	
// 	$docId = wf_cadastrarDocumento ( WORKFLOW_TIPODOCUMENTO_PAGAMENTO, 'Fluxo do pagamento de estudante' );
	
// 	if (! $VerificaPagamento['pgeid']) {
// 	$sqlInserePagamento = "INSERT INTO projovemurbano.pagamentoestudante( caeid, diaid, pgeaptoreceber, docid )
// 	VALUES( {$aluno['caeid']}, {$diaid}, '{$apto}', {$docId}) RETURNING pgeid";
// 	$db->pegaUm ( $sqlInserePagamento );
	
// 	$sqlAtualizaAptoAReceber = "SELECT projovemurbano.fn_situacao_aluno_pagto({$aluno['caeid']}, {$diaid});";
// 	$db->pegaUm ( $sqlAtualizaAptoAReceber );
		
// 	}elseif($VerificaPagamento['pgeid'] &&  $VerificaPagamento['pgeaptoreceber']!='t'&& $VerificaPagamento['pgeaptoreceber']!= $apto){
		
// 		$atualizapagamento = "UPDATE projovemurbano.pagamentoestudante
// 							  SET pgeaptoreceber = 't',
// 								  docid = {$docId}
// 							  WHERE
// 							  	  caeid = {$aluno['caeid']}
// 							  AND diaid = {$diaid}";
// 		$db->pegaUm ( $atualizapagamento );
		
// 		$sqlAtualizaAptoAReceber = "SELECT projovemurbano.fn_situacao_aluno_pagto({$aluno['caeid']}, {$diaid});";
// 		$db->pegaUm ( $sqlAtualizaAptoAReceber );
// 	}
	
// 	}
// 	// ver($sqlInserePagamento);
// 		$db->commit ();
// 		echo "<script>
// 			alert('testado!');
// 	    </script>";
// 		return true;
// 	}
	
	/**********/
if($_REQUEST['requisicao']) {
// 	ver(header('content-type: text/html; charset=ISO-8859-1'),d);
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


checkAno();

include  APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';
echo '<br>';

$menu = array(0 => array("id" => 1, "descricao" => "Gerar Arquivo CEF", 			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/gerenciarNIS&acao=A"),
				  1 => array("id" => 2, "descricao" => "Carregar Arquivo NIS", 	"link" => "/projovemurbano/projovemurbano.php?modulo=principal/carregarNIS&acao=A")
			  	  );

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

monta_titulo('Projovem Urbano', 'Carregar NIS. Selecione o arquivo abaixo.');

$bloq = 'S';
$disabled = '';


?>
<script>

function atualizarReceita() {
	document.getElementById('requisicao').value='atualizarReceita';
	document.getElementById('formulario').submit();
}
function verificaAlunoApto() {
	document.getElementById('requisicao').value='verificaAlunoApto';
	document.getElementById('formulario').submit();
}
function atualizarEndereco() {
	document.getElementById('requisicao').value='atualizarEndereco';
	document.getElementById('formulario').submit();
}
function atualizarBolsas() {
	document.getElementById('requisicao').value='atualizarBolsas';
	document.getElementById('formulario').submit();
}
function enviarCarga() {
	if(document.getElementById('arquivocsv').value == '') {
		alert('Selecione um arquivo para enviar');
		return false;
	}
	document.getElementById('formulario').submit();
}

function desfazer() {
	if(document.getElementById('caeid').value == '') {
		alert('Ensira uma matrícula!');
		return false;
	}
	document.getElementById('requisicao').value='desfazertransferencia';
	document.getElementById('formulario').submit();
}

function atualizarCarga() {
	if(document.getElementById('perid').value == '') {
		alert('Escolha um periodo!');
		return false;
	}
	document.getElementById('requisicao').value='atualizapagamento';
	document.getElementById('formulario').submit();
}
function atualizarPresenca() {
	if(document.getElementById('perid1').value == '') {
		alert('Escolha um periodo!');
		return false;
	}
	document.getElementById('requisicao').value='atualizapresenca';
	document.getElementById('formulario').submit();
}

function atualizarTrans() {
	document.getElementById('requisicao').value='atualizatransferencia';
	document.getElementById('formulario').submit();
}
function deletarAluno() {
	if(document.getElementById('caecpf').value == '') {
		alert('Ensira uma cpf!');
		return false;
	}
	document.getElementById('requisicao').value='deletarAluno';
	document.getElementById('formulario').submit();
}

</script>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="" target="iframeUpload">
	<input type="hidden" name="requisicao" value="executarCarga" id="requisicao">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<b>Carregar arquivos NIS</b>
			</td>
		</tr>
		<tr>
			<td width="40%" class="SubTituloDireita">Arquivo:</td>
			<td>
				<input type="file" name="arquivo" id="arquivocsv">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td class="SubTituloEsquerda">
				<input type="button" value="Enviar" onclick="enviarCarga();">
				<?if($_SESSION['usucpf'] == ''||$_SESSION['usucpf'] == ''){?>
					<input type="button" value="Atualizar Bolsas" onclick="atualizarBolsas();">
					<input type="button" value="Atualizar Transferencia" onclick="atualizarTrans();">
					
				<?}?>
			</td>
		</tr>
	 	<?if($_SESSION['usucpf'] == ''||$_SESSION['usucpf'] == ''){?>
		 	 <tr>
		 	 	<td class="SubTituloCentro" colspan="3">Ao se utilizar essa ferramenta deve ter em mente que os dados do aluno voltaram a ser o que eram antes, como se ele nunca houvesse sido transferido.</td>
		 	 </tr>
			 <tr>
		 	 	<td class="SubTituloDireita">Matrícula:</td>
		    	<td><? echo campo_texto('caeid', "S", 'S', "Matrícula", 16, 14, "#########", "", '', '', 0, 'id="caeid" required', '', '', '' ); ?></td>
	  		</tr>
	    	<tr>
	    		<td class="SubTituloCentro" colspan="3"><input type="button" value="Desfazer transferência" onclick="desfazer();"></td>
	    	</tr>
	    	<tr>
	    		<td class="SubTituloCentro" colspan="3"><input type="button" value="Atualizar Endereço" onclick="atualizarEndereco();"></td>
	    	</tr>
<!-- 	    	<tr> -->
<!--	    		<td class="SubTituloCentro" colspan="3"><input type="button" value="Testa Aluno" onclick="verificaAlunoApto();"></td>-->
<!-- 	    	</tr> -->
	    	<tr>
	    		<td class="SubTituloDireita">
	    			Período:
	    		</td>
	    		<td>
		    		<? $sql = "
							SELECT 
								perid as codigo,
								perid as descricao
							FROM 
								projovemurbano.periodocurso ";
					$db->monta_combo('perid', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'perid');  ?>
					<input type="button" value="Atualizar Carga" onclick="atualizarCarga();">
	    		</td>
	    	</tr>	
	    	<tr>
	    		<td class="SubTituloDireita">
	    			Período:
	    		</td>
	    		<td>
		    		<? $sql = "
							SELECT 
								perid as codigo,
								perid as descricao
							FROM 
								projovemurbano.periodocurso
							WHERE
								ppuid = 2 ";
					$db->monta_combo('perid1', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'perid1');  ?>
					<input type="button" value="Atualizar Presença" onclick="atualizarPresenca();">
	    		</td>
	    	</tr>	
	    <?}?>
	    <?if(in_array(PFL_ADMINISTRADOR, $perfis)||$db->testa_superuser()){?>
	    	 <tr>
		 	 	<td class="SubTituloDireita">CPF:</td>
		    	<td><? echo campo_texto('caecpf', "S", 'S', "CPF", 16, 14, "#########", "", '', '', 0, 'id="caecpf" required', '', '', '' ); ?></td>
	  		</tr>
	    	<tr>
	    		<td class="SubTituloCentro" colspan="3"><input type="button" value="Deletar aluno" onclick="deletarAluno();"></td>
	    	</tr>
	    	 <tr>
		 	 	<td class="SubTituloDireita">CPF:</td>
		    	<td><? echo campo_textarea("cpf","N","S","Digite o outro motivo", 60, 10,10000,"","","","","",""); ?></td>
	  		</tr>
	    	<tr>
	    		<td class="SubTituloCentro" colspan="3"><input type="button" value="Atualizar Dados Receita" onclick="atualizarReceita();"></td>
	    	</tr>
	    <?}?>
</table>

<iframe name="iframeUpload" id="iframeresp" style="width:100%;height:800px;border:0px solid #fff;"></iframe>

</form>


