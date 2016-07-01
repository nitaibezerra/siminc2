<?php
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/funcoesspo.php";
include_once APPRAIZ . "demandasfies/classes/html_table.class.php";

class Model_Grupopet extends Abstract_Model
{

	protected $_schema = 'pet';
	protected $_name = 'grupopet';
	public $entity = array();
	public $tabelaArvore;

	public function __construct($commit = true)
	{
		$this->tabelaArvore =
		parent::__construct($commit);

		$this->entity['grpid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
		$this->entity['iesid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
		$this->entity['abrangencia'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
		$this->entity['nomegrupo'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '500', 'contraint' => '');

	}

	public function getSqlLista()
	{
		$sql = " SELECT gp.grpid, gp.nomegrupo,
                    CASE
                           WHEN abrangencia = 'C' THEN 'CURSO ESPECIFICO'
                           WHEN abrangencia = 'I' THEN 'INTERDISCIPLINAR'
                    END AS abrangencia,
                    coalesce ( ( (LENGTH(numeroeixorespondido) - LENGTH(REPLACE(numeroeixorespondido, '1', '')) ) * 100 ) / char_length(numeroeixorespondido) || '%' , '0%' ) AS per_preenc,
                    CASE
                           WHEN finalizado = 't'
    					   		THEN 'FINALIZADO'
    					   		ELSE 'EM PREENCHIMENTO'
                    END AS finalizado
                FROM pet.grupopet  AS gp
                LEFT JOIN pet.identificacaogrupo AS idg ON  idg.grpid = gp.grpid
                LEFT JOIN pet.consideracoesfinais AS confin ON  confin.idgid = idg.idgid
                WHERE iesid = {$_SESSION['dadosResposabilidade']['iesid']}
                ORDER BY gp.nomegrupo
                ";
		return $sql;
	}

	public function getListaCursos()
	{
		if(!empty($_SESSION['dadosResposabilidade']['iesid'])){
			$sql = $this->getSqlLista();
			$dados = $this->_db->carregar($sql);
			$dados = $dados ? $dados : array();

			$listagem = new Listing(false);
			$listagem->setPerPage(30);
			$listagem->setActions(array('share-alt' => 'selecionar'));
			$listagem->setHead(array('Grupo', 'Abrangência', 'Questões Respondidas','Status'));
			$listagem->setEnablePagination(false);
			$listagem->listing($dados);
		}else{
			echo "<div  class='alert alert-warning'>O usuário não possui nenhuma Instituição de Ensino Superior em sua responsabilidade.</div>";
		}
	}

	public function getSqlUniversidade()
	{
		return " SELECT confin.cofid, idgrupo.nome, gp.nomegrupo,
                   CASE
                           WHEN abrangencia = 'C' THEN 'CURSO ESPECIFICO'
                           WHEN abrangencia = 'I' THEN 'INTERDISCIPLINAR'
                    END AS abrangencia,
                    coalesce ( ( (LENGTH(numeroeixorespondido) - LENGTH(REPLACE(numeroeixorespondido, '1', '')) ) * 100 ) / char_length(numeroeixorespondido) || '%' , '0%' ) AS per_preenc,
                    CASE
                           WHEN confin.finalizado = 't' THEN 'FINALIZADO'
                           WHEN confin.finalizado = 'f' THEN 'EDITÁVEL'
                    END AS situacao
                FROM pet.grupopet  AS gp
                LEFT JOIN pet.identificacaogrupo AS idg ON  idg.grpid = gp.grpid
                LEFT JOIN pet.consideracoesfinais AS confin ON  confin.idgid = idg.idgid
                LEFT JOIN pet.institutoensinosuperior AS idgrupo ON  idgrupo.iesid = gp.iesid
                ";
	}

	public function getListaPainel()
	{
		$sqlUniversidades = $this->getSqlUniversidade();
		$sqlQuestionario = $this->getSqlQuestionario();

		$camposDaTabela = $this->getCamposQuestionario();
		$camposQuestionario = $this->getCamposDaTabela();
	}

	public function getInformacoesBasicas($grpid)
	{
		$sql = "SELECT
                    gp.grpid, gp.nomegrupo,
                        CASE
                               WHEN abrangencia = 'C' THEN 'CURSO ESPECIFICO'
                               WHEN abrangencia = 'I' THEN 'INTERDISCIPLINAR'
                        END AS abrangencia,
                    instsup.nome AS instuicaoEnsinoSuperior,
                    tut.nome as nomeTutor,
                    tut.cpf as cpfTutor,
                    tut.datainiciotutoria,
                    cur.nome AS nomeCurso
                    FROM pet.grupopet AS gp

                    LEFT JOIN pet.identificacaogrupo AS idg ON  idg.grpid = gp.grpid
                    INNER JOIN pet.institutoensinosuperior AS instsup ON  instsup.iesid = gp.iesid
                    INNER JOIN pet.tutor AS tut ON  tut.grpid = gp.grpid
                    LEFT JOIN pet.grupopetcurso AS gpcur ON  gpcur.grpid = gp.grpid
                    LEFT JOIN pet.curso AS cur ON cur.curid = gpcur.curid
                    WHERE gp.grpid =  {$grpid};
                ";
		$dados = $this->_db->carregar($sql);
		$dados = $this->tratarDados($dados);
		$dados = $dados ? $dados : array();
		return $dados;
	}

	public function tratarDados($dados)
	{
		if ($dados) {
			$newData = array();
			foreach ($dados as $key => $valor) {
				$newData['grpid'] = $valor['grpid'];
				$newData['nomegrupo'] = $valor['nomegrupo'];
				$newData['abrangencia'] = $valor['abrangencia'];
				$newData['instuicaoEnsinoSuperior'] = $valor['instuicaoensinosuperior'];
				$newData['nometutor'] = $valor['nometutor'];
				$newData['cpftutor'] = formatar_cpf($valor['cpftutor']);
				$newData['datainiciotutoria'] = formata_data($valor['datainiciotutoria']);
				$newData['nomecurso'][$key] = $valor['nomecurso'];
			}
			return $newData;
		}
	}

	function getCamposQuestionario()
	{
		return array(
			'dataabertura' => 'Data Abertura',
			'dataencerramento' => 'Data Encerramento',
			'titulo' => 'Título'
		);
	}

}
