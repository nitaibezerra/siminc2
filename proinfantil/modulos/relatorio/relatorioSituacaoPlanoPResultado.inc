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

            $sql = "SELECT
		        tm.estuf as \"Estado\",
		        tm.mundescricao as \"Município\",
		        count(*) as \"Quantidade de Obras\",
		        edoc.esddsc as \"Situação\"
		        FROM
				obras2.obras AS oi
                INNER JOIN obras2.empreendimento e ON e.empid =  oi.empid
				INNER JOIN entidade.entidade AS ee ON oi.entid= ee.entid
				INNER JOIN entidade.endereco AS ed ON oi.endid = ed.endid
                INNER JOIN territorios.municipio AS tm ON ed.muncod = tm.muncod
                INNER JOIN obras2.orgao AS oo ON e.orgid = oo.orgid AND
                								 oo.orgstatus = 'A'                                                 
                INNER JOIN workflow.documento d1 ON d1.docid = oi.docid
                INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid
                INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
                INNER JOIN obras2.tipoobra AS tp ON oi.tobid = tp.tobid
                LEFT JOIN obras2.tipologiaobra AS tpl ON oi.tpoid = tpl.tpoid AND tpl.tpostatus = 'A'
                LEFT JOIN proinfantil.proinfantil pi ON pi.obrid = oi.obrid
                LEFT JOIN (SELECT 		q.pinid, r.resdsc
                           FROM 		proinfantil.questionario q
                           INNER JOIN 	questionario.resposta r on r.qrpid = q.qrpid
                           WHERE	 	r.perid = 1587 ) as dt ON dt.pinid = pi.pinid				
                LEFT JOIN workflow.documento d ON d.docid = pi.docid
                LEFT JOIN workflow.estadodocumento edoc on edoc.esdid = d.esdid
			WHERE
			    oi.obrstatus = 'A' AND
			    ee.entstatus = 'A' AND
			    pf.prfid = 41 AND
			    oi.obrpercentultvistoria >= 90 AND
                esd1.esdid IN (690, 693) and
			    oi.obridpai IS NULL
			     and tpl.tpoid in (16,9,10,104,105) ";

            if ($situacao) {
                $sql.= " AND edoc.esdid IN (' {$situacao} ')";
            }

            if ($strMunCod) {
                $sql.= "AND
						tm.muncod in ( {$strMunCod} )
						
						";
            }
            if ($strUf) {
                $sql.= "AND
						tm.estuf in ( {$strUf} )
						
						";
            }

            $sql .= "GROUP BY 
			        tm.estuf, tm.mundescricao, edoc.esddsc ORDER BY tm.estuf, tm.mundescricao, edoc.esddsc ";
            
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
        $file_name = "relatorioSituacaoPlanoProinfancia.xls";
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
        monta_titulo('Relatório de Situação do Plano - Proinfância', '');
    }
    // Monta tabela que irá exibir os dados
    echo '<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">';
    // Cabeçalho para o arquivo excel
    if ($isXls) {
        echo ' <tr style="background-color: #dcdcdc;"> <td colspan="' . count($dados[0]) . '" > Relatório de Situação do Plano - Proinfância </td></tr>';
    }

    // Começa linha com os nomes das colunas
    echo '	<tr >';

    foreach ($dados[0] as $colKey => $colVal) {
        echo "		<td style=\"background-color: #e9e9e9;\" align=\'center\'  > <b> {$colKey} </b> </td>";
    }
    echo '</tr>';

    // Lista as informações do relatório
    foreach ($dados as $linhaKey => $linhaVal) {
        $qnt += $linhaVal['Quantidade de Obras'];
        echo '<tr>';
        foreach ($linhaVal as $colKey => $colVal) {
            echo " <td align=\'center\' width=\'50%\' > <b> {$colVal} </b> </td>";
        }
    }
    // Finaliza o relatório
    echo '<tr><td style="background-color: #e9e9e9;" align="center" colspan="4"><b>Total de Obras: '.$qnt.'</b></table>';

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