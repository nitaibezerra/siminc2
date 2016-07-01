<?php

/**
 * Controle responsavel pelas entidades.
 *
 * @author Equipe simec - Consultores OEI
 * @since  17/10/2013
 *
 * @name       Board
 * @package    classes
 * @subpackage controllers
 * @version    $Id
 */
class Controller_Indicadoreseducacionais extends Abstract_Controller
{
    public function censoEducacional2012Action()
    {
        global $clsOracle;

$clsGrafico = new Grafico(Grafico::K_TIPO_PIZZA , false);

$intBarramento = $_SESSION['instituicao']['intidbarramento'];

$sql = "
        SELECT

          --ies.NO_IES || ' - ' || ies.SG_IES as descricao ,
        --ies.NO_IES,
        moda.NO_MODALIDADE_ENSINO  as DESCRICAO,
          --ies.NO_IES,
          --ies.SG_IES,
          --curso.NO_CURSO,
          --curso.NU_CARGA_HORARIA,
          --curso.CO_DM_MODALIDADE_ENSINO,
          count(moda.NO_MODALIDADE_ENSINO) as VALOR
          --curso.DT_INICIO_FUNCIONAMENTO,
          --loof.NO_LOCAL_OFERTA,
          --loof.NO_BAIRRO,
          --loof.DS_LOGRADOURO,
          --loof.NU_CEP

        from
          ODSCENSOSUP_2012.DM_IES ies
          INNER JOIN ODSCENSOSUP_2012.DM_LOCAL_OFERTA_IES loof ON ies.CO_DM_IES = loof.CO_DM_IES
          INNER JOIN ODSCENSOSUP_2012.DM_CURSO curso ON loof.CO_DM_LOCAL_OFERTA_IES = curso.CO_DM_LOCAL_OFERTA_IES
          INNER JOIN ODSCENSOSUP_2012.DM_MODALIDADE_ENSINO moda ON curso.CO_DM_MODALIDADE_ENSINO = moda.CO_DM_MODALIDADE_ENSINO
        WHERE ies.CO_CATEGORIA_ADMINISTRATIVA = 211
        AND ies.CO_DM_IES = {$intBarramento}
        AND curso.CO_DM_NIVEL_ACADEMICO = 1
        group by ies.NO_IES , moda.NO_MODALIDADE_ENSINO";

$sqlCor = "
        -- Cor
        SELECT CR.NO_COR_RACA AS descricao , COUNT (CR.CO_DM_COR_RACA) AS valor
        --*
        FROM ODSCENSOSUP_2012.DM_IES I
        INNER JOIN ODSCENSOSUP_2012.DM_CURSO C ON (C.CO_DM_IES = I.CO_DM_IES)
        INNER JOIN ODSCENSOSUP_2012.FT_MATRICULA M ON (M.CO_DM_CURSO = C.CO_DM_CURSO)
        INNER JOIN ODSCENSOSUP_2012.DM_ALUNO A ON (A.CO_DM_ALUNO = M.CO_DM_ALUNO)
        INNER JOIN ODSCENSOSUP_2012.DM_COR_RACA CR ON (CR.CO_DM_COR_RACA = A.CO_DM_COR_RACA)
        WHERE I.CO_DM_IES = {$intBarramento}
        AND I.CO_CATEGORIA_ADMINISTRATIVA = 211
        AND C.CO_DM_NIVEL_ACADEMICO = 1
        GROUP BY CR.NO_COR_RACA , CR.CO_DM_COR_RACA";
$sqlSituacao = "
        -- Situação
        SELECT S.NO_ALUNO_SITUACAO as descricao , COUNT (S.CO_DM_ALUNO_SITUACAO) AS valor
        --*
        FROM ODSCENSOSUP_2012.DM_IES I
        INNER JOIN ODSCENSOSUP_2012.DM_CURSO C ON (C.CO_DM_IES = I.CO_DM_IES)
        INNER JOIN ODSCENSOSUP_2012.FT_MATRICULA M ON (M.CO_DM_CURSO = C.CO_DM_CURSO)
        INNER JOIN ODSCENSOSUP_2012.DM_ALUNO_SITUACAO S ON (S.CO_DM_ALUNO_SITUACAO = M.CO_DM_ALUNO_SITUACAO)
        WHERE I.CO_DM_IES = {$intBarramento}
        AND I.CO_CATEGORIA_ADMINISTRATIVA = 211
        AND C.CO_DM_NIVEL_ACADEMICO = 1
        GROUP BY S.NO_ALUNO_SITUACAO , S.CO_DM_ALUNO_SITUACAO";

        $resultados = $clsOracle->getAll($sql);
        $resultadosCor = $clsOracle->getAll($sqlCor);
        $resultadosSituacao = $clsOracle->getAll($sqlSituacao);
//ver($sql, $sqlSituacao , $sqlCor , d);
//Situação
// Raça

        foreach($resultados as $key => &$resultado)
        {
            $resultado['descricao'] = $resultado['DESCRICAO'];
            $resultado['valor'] = $resultado['VALOR'];

            unset($resultado['DESCRICAO']);
            unset($resultado['VALOR']);
        }

        foreach($resultadosCor as $key => &$resultadoCor)
        {
            $resultadoCor['descricao'] = $resultadoCor['DESCRICAO'];
            $resultadoCor['valor'] = $resultadoCor['VALOR'];

            unset($resultadoCor['DESCRICAO']);
            unset($resultadoCor['VALOR']);
        }

        foreach($resultadosSituacao as $key => &$resultadoSituacao)
        {
            $resultadoSituacao['descricao'] = $resultadoSituacao['DESCRICAO'];
            $resultadoSituacao['valor'] = $resultadoSituacao['VALOR'];

            unset($resultadoSituacao['DESCRICAO']);
            unset($resultadoSituacao['VALOR']);
        }
//        ver($resultados , $resultadosCor , $resultadosSituacao, d);

//        $clsGrafico->setAgrupadores(array('descricao' => 'DESCRICAOS', 'valor' => 'VALORSS'))->setTitulo('Modalidade de Ensino')->gerarGrafico($resultados);
        echo "<div class='row'><div class='col-lg-4'>";
            $clsGrafico->setHeight('450px')->setTitulo('Modalidade de Ensino')->gerarGrafico($resultados);
        echo "</div>";
        echo "<div class='col-lg-4'>";
            $clsGrafico->setHeight('450px')->setTitulo('Situação - Matrículas')->gerarGrafico($resultadosSituacao);
        echo "</div>";
        echo "<div class='col-lg-4'>";
            $clsGrafico->setHeight('450px')->setTitulo('Etnia')->gerarGrafico($resultadosCor);
        echo "</div>";
        echo "</div>";
    }

    public function censoEducacional2013Action()
    {
        echo __METHOD__;
    }

    public function ideAction()
    {
        echo __METHOD__;
    }

    public function igcAction()
    {
        echo __METHOD__;
    }


    public function indexAction()
    {
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function exibirAction()
    {
        $this->render(__CLASS__, __FUNCTION__);
    }
}