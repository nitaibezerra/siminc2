<?php

// Codificação do relatório
header('Content-Type: text/html; charset=iso-8859-1');
// Objeto da classe do relatório dos dados da unidade
$obRelatorio = new relatorioNovasTurmasResultado();
// Verifica se é um relatório p o browser ou excel
$isXls = ($_REQUEST['requisicao'] == 'relatorio_xls') ? true : false;

// Classe responsável por montar o resultado do relatorio
class relatorioNovasTurmasResultado {

    // Variáveis da classe
    public $esfera = '';
    public $arrTipoEntidade = array();
    public $requisicao = '';
    public $arrColunas = array();
    public $arrUfs = array();
    public $arrMunicipios = array();
    public $dados = array();
    public $db = '';

    // Método construtor
    function __construct() {
        // Instancia a classe do BD
        $this->db = new cls_banco();

        set_time_limit(0);

        // Verifica a requisição (relatorio ou relatorio_xls)
        $this->requisicao = $_POST['requisicao']; // 
        // Verifica as colunas escolhidas para o relatório
        $this->arrColunas = $this->_POST['coluna'];
    }

    /**
     * Monta a string apartir do array e retorna no formato do sql
     * */
    public function getArrToStrParam($arrParam) {
        if ($arrParam[0] != '') {
            foreach ($arrParam as $k => $v) {
                $arrParam[$k] = "'{$v}'";
            }

            $strReturn = implode(",", $arrParam);
        } else {
            $strReturn = false;
        }
        return $strReturn;
    }

    function monta_sql() {

        $situacao = implode("','", $_REQUEST['situacao']);
        // Carrega as colunas
        $arrColunas = $_POST['coluna'];
        // Recupera array com as ufs
        $arrUf = $_POST['estuf'];
        // Monta o parametro para a clausula where
        $strUf = $this->getArrToStrParam($arrUf);
        // Recupera array com os município
        $arrMunCod = $_POST['muncod'];
        // Monta o parametro para a clausula where 		
        $strMunCod = $this->getArrToStrParam($arrMunCod);
        // Ano exercício
        $anoExercicio = (int) $_SESSION['exercicio'];
        // Ano censo
        $anoCenso = ((int) $_SESSION['exercicio'] - 1);
        $ano = $_REQUEST['ano'];

            $sql = " SELECT  mun.estuf as \"Estado\", mun.mundescricao as \"Município\", count(*) as \"Quantidade de Turmas\", esd1.esddsc as \"Situação\"
				 FROM	    	proinfantil.novasturmasworkflowturma ntw
				 inner join workflow.documento doc ON doc.docid = ntw.docid
				 INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = doc.esdid 
				 inner join proinfantil.turma t on t.turid = ntw.turid and t.turstatus = 'A'  
				 --INNER JOIN entidade.endereco ede ON ede.endid = mun.endid
				INNER JOIN territorios.municipio mun ON mun.muncod = ntw.muncod
                                WHERE 1 = 1";

            if ($situacao) {
                $sql.= "AND
						esd1.esdid  in ('{$situacao}')
						";
            }

            if ($strMunCod) {
                $sql.= "AND
						mun.muncod in ( {$strMunCod} )
						
						";
            }
            if ($strUf) {
                $sql.= " AND
						mun.estuf in ( {$strUf} )
						
						";
            }
            
            if ($ano) {
                $sql.= " AND t.turano = {$ano} ";
            }else{
               $sql.=  " AND t.turano in ('2013', '2014', '2015')";
            }

            $sql .= " group by 	 mun.estuf, mun.mundescricao, esd1.esddsc
order by  mun.estuf, mun.mundescricao, esd1.esddsc";
           // ver($sql,d);
            return $sql;
    }

    function retornaDados() {
        $sql = $this->monta_sql();
        if ($sql) {

            return $this->dados = $this->db->carregar($sql);
        } else {
            return array();
        }
    }

}
?>
<html>
    <head>

<?php
if (!$isXls) {
    ?>
            <script type="text/javascript" src="../includes/funcoes.js"></script>
            <link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
            <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
}
?>

    </head>
    <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
// Faz requisição para o método retornaDados que irá retornar o array com os dados do relatório
$dados = $obRelatorio->retornaDados();

// Caso exista dados ele  monta o relatório, senão exibe mensagem de que não existem registros
if (is_array($dados) && count($dados)) {
    // Caso seja uma exportação para excel
    if ($isXls) {
        $file_name = "relatorioSituacaoPlano.xls";
        header("Pragma: public");
        header("Expires: 0");
        header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
        header("Content-Type: application/force-download");
        header("Content-Type: application/octet-stream");
        header("Content-Type: application/download");
        header("Content-Disposition: attachment;filename=" . $file_name);
        header("Content-Transfer-Encoding: binary ");
    }

    // Monta o cabeçalho com o Brasao
    $cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
    $cabecalhoBrasao .= "<tr>" .
            "<td colspan=\"100\">" .
            monta_cabecalho_relatorio('100') .
            "</td>" .
            "</tr>
				        </table>";

                        
    // Caso não seja excel monta cabeçalho e título						
    if (!$isXls) {
        echo $cabecalhoBrasao;
        monta_titulo('Relatório de Situação do Plano - Novas Turmas', '');
    }
    // Monta tabela que irá exibir os dados
    echo '<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">';
    // Cabeçalho para o arquivo excel
    if ($isXls) {
        echo ' <tr style="background-color: #dcdcdc;"> <td colspan="' . count($dados[0]) . '" > Relatório de Situação do Plano - Novas Turmas </td></tr>';
    }

    // Começa linha com os nomes das colunas
    echo '	<tr >';

    foreach ($dados[0] as $colKey => $colVal) {
        echo "		<td style=\"background-color: #e9e9e9;\" align=\'center\'  > <b> {$colKey} </b> </td>";
    }
    echo '</tr>';

    // Lista as informações do relatório
    foreach ($dados as $linhaKey => $linhaVal) {
        $qnt += $linhaVal['Quantidade de Turmas'];
        echo '<tr>';
        foreach ($linhaVal as $colKey => $colVal) {
            echo " <td align=\'center\' width=\'50%\' > <b> {$colVal} </b> </td>";
        }
    }
    // Finaliza o relatório
    echo '<tr><td style="background-color: #e9e9e9;" align="center" colspan="4"><b>Total de Turmas: '.$qnt.'</b></table>';
} else {
    // Exibe mensagem de que não existem registros.
    die('<table cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">' .
            '<TR style="background:#FFF; color:red;">' .
            '<TD colspan="10" align="center">' .
            'Não foram encontrados registros.' .
            '</TD>' .
            '</TR>' .
            '</table>');
}

if ($isXls) {
    // Finaliza processo
    die('<script>window.close();</script>');
}
?>

    </body>