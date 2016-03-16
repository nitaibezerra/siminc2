<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$( document ).ready(function() {

    $('.tabela > tbody > tr ').each(function() {
        var linha = $(this);
        //recuperando html das td posicao eq(1)
        var srtConteudoLinha = linha.find('td:eq(1)').html();
        //tratando html para o replace não remover a img do brasao, quando string html for diferente de null e não encontrar img, aplicar o replace
        if(srtConteudoLinha != null && srtConteudoLinha.search('img') == -1){
            linha.find('td:eq(1)').text(linha.find('td:eq(1)').text().replace(',00', ''));//tratando valores float para integer
        }
    });
});
</script>

<?php //die();// não recarregar a tela pai ?>



<?PHP

    set_time_limit(0);

    if ( $_REQUEST['filtrosession'] ){
            $filtroSession = $_REQUEST['filtrosession'];
    }

    if ($_POST['agrupador']){
            header( 'Content-Type: text/html; charset=iso-8859-1' );
    }
?>
    <html>
        <head>
            <script type="text/javascript" src="../includes/funcoes.js"></script>

            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
            <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        </head>

        <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
<?PHP

    include APPRAIZ. 'includes/classes/relatorio.class.inc';

    $sql   = monta_sql( $_POST['req'] );
    $dados = $db->carregar($sql);
    $agrup = monta_agp();
    $col   = monta_coluna();

    $r = new montaRelatorio();
    $r->setAgrupador($agrup, $dados);
    $r->setColuna($col);
    $r->setBrasao($true ? true : false);
    $r->setMonstrarTolizadorNivel(true);
    $r->setTotNivel(true); 
    $r->setEspandir( $_POST['expandir'] );

    if( $_POST['req'] == 2 ){
        ob_clean();
        $nomeDoArquivoXls="Relatório_gerencial_eja_novas_turmas".date('d-m-Y_H_i');
        echo $r->getRelatorioXls();
    }else{
        echo $r->getRelatorio();
    }

    function monta_sql( $rel_tipo ){
        
        if( $_POST['estuf'] != '' ){
            $where[] = " est.estuf = '{$_POST['estuf']}' ";
        }
        
        if( $_POST['muncod'] != '' ){
            $where[] = " mun.muncod = '{$_POST['muncod']}' ";
        }
        
        if( $_POST['ntefechaturma'] != '' ){
            $where[] = " nte.ntefechaturma = '{$_POST['ntefechaturma']}' ";
        }
        
        if( $_POST['nteenviofnde'] != '' ){
            $where[] = " nte.nteenviofnde = '{$_POST['nteenviofnde']}' ";
        }
        
        if( $where != '' ){
            $where = " AND ".implode(' AND ', $where);
        }
        
        #MANOBRA PARA "MINIMIZAR" PROBLEMA COM O RELATÓRIO, DUPLICAÇÃO DE DIRETORES.
        $colunas = $_POST['colunas'];
        foreach ($colunas as $value){
            if( $value == 'usunome' ){
                $col_diret_existe = "S";
            }
        }
        
        if( $col_diret_existe != 'S' ){
            $limit = "LIMIT 1";
        }
        
        if ($_REQUEST['ano']) {
                $anoQuery = $_REQUEST['ano'];
            } else {
                $anoQuery = '2015';
            }
            
	$sql = "
            SELECT  ate.ateid,
                    est.estuf,
                    mun.mundescricao,
                    ent.entnome,
                    'Turma - '||nte.ntesequencial AS turma_escola,
                    su.usunome,
                    atecpf,
                    atenomedoaluno,
                    ateexercicio AS ano_exercicio

            FROM  eja.alunoturmaeja ate

            JOIN eja.novaturmaeja nte ON nte.nteid = ate.nteid
            JOIN entidade.entidade ent ON ent.entid = nte.pk_cod_entidade

            LEFT JOIN(
                SELECT entid, usucpf FROM eja.usuarioresponsabilidade WHERE rpustatus = 'A' {$limit}
            ) AS usu ON usu.entid = nte.pk_cod_entidade
            
            LEFT JOIN seguranca.usuario su ON su.usucpf = usu.usucpf

            JOIN entidade.endereco ende ON ende.entid = ent.entid
            JOIN territorios.municipio mun ON mun.muncod = ende.muncod
            JOIN territorios.estado est ON est.estuf = mun.estuf

            LEFT JOIN(
                SELECT e.entnumcpfcnpj, ende.muncod
                FROM entidade.entidade e
                JOIN entidade.endereco ende ON ende.entid = e.entid
                JOIN entidade.funcaoentidade f on f.entid = e.entid AND f.funid = 1
            ) AS cnpj on cnpj.muncod = ende.muncod

            WHERE ent.entstatus = 'A' AND nte.ntestatus = 'A' AND (atecpf <> '' OR atecpf IS NOT NULL) {$where}
            and ateexercicio = '{$anoQuery}'
            ORDER BY est.estuf, mun.mundescricao, entnome, turma_escola, atenomedoaluno
        ";
//            ver($sql,d);
	return $sql;
    }

    function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];

	$agp = array(
            "agrupador" => array(),

            "agrupadoColuna" => array(
                    "mundescricao",
                    "entnome",
                    "turma_escola",
                    "usunome",
                    "atecpf",
                    "atenomedoaluno",
                    "ano_exercicio",
                    "estuf"
            )
        );

        array_push($agp['agrupador'], array( "campo" => "estuf", "label" => "Estado"));
        
	foreach ($agrupador as $val){
            switch ($val) {
                case 'mundescricao':
                    array_push($agp['agrupador'], array( "campo" => "mundescricao", "label" => "Município") );
                    continue;
                case 'entnome':
                    array_push($agp['agrupador'], array( "campo" => "entnome", "label" => "Escola") );
                    continue;
                case 'turma_escola':
                    array_push($agp['agrupador'], array( "campo" => "turma_escola", "label" => "Turma") );
                    continue;
                case 'usunome':
                    array_push($agp['agrupador'], array( "campo" => "usunome", "label" => "Diretor") );
                    continue;
                case 'atecpf':
                    array_push($agp['agrupador'], array( "campo" => "atecpf", "label" => "CPF do Aluno") );
                    continue;
                case 'atenomedoaluno':
                    array_push($agp['agrupador'], array( "campo" => "temdescricao", "label" => "Aluno") );
                    continue;
                case 'ano_exercicio':
                    array_push($agp['agrupador'], array( "campo" => "ano_exercicio", "label" => "Ano do Exercício") );
                    continue;
            }
        }
        array_push($agp['agrupador'], array( "campo" => "ateid", "label" => "Cód. Aluno" ) );
        
	return $agp;
    }

    function monta_coluna(){

	$colunas = $_POST['colunas'];
	$colunas = $colunas ? $colunas : array();

	$coluna = array();

	foreach ($colunas as $val){
            switch ($val) {
                case 'estuf':
                    array_push( $coluna, array( "campo" => "estuf", "label" => "Estado", "type" => "string" ) );
                    continue;
                break;
                case 'mundescricao':
                    array_push( $coluna, array( "campo" => "mundescricao", "label" => "Município", "type" => "string" ) );
                    //array_push( $coluna, array( "campo" => "sersiape", "label" => "SIAPE", "type" => "string", "html" => "<div style=\"text-align: left;\">{sernome}</div>") );
                    continue;
                break;
                case 'mundescricao':
                    array_push( $coluna, array( "campo" => "mundescricao", "label" => "Município", "type" => "string" ) );
                    continue;
                break;
                case 'entnome':
                    array_push( $coluna, array( "campo" => "entnome", "label" => "Escola", "type" => "string" ) );
                    continue;
                break;
                case 'turma_escola':
                    array_push( $coluna, array( "campo" => "turma_escola", "label" => "Turma", "type" => "string" ) );
                    continue;
                break;
                case 'usunome':
                    array_push( $coluna, array( "campo" => "usunome", "label" => "Diretor", "type" => "string" ) );
                    continue;
                break;
                case 'atecpf':
                    array_push( $coluna, array( "campo" => "atecpf", "label" => "CPF Aluno", "type" => "string" ) );
                    continue;
                break;
                case 'atenomedoaluno':
                    if( $_POST['req'] == 2 ){
                    array_push( $coluna, array( "campo" => "atenomedoaluno", "label" => "Aluno", "type" => "string" ) );
                    }else{
                    array_push( $coluna, array( "campo" => "atenomedoaluno", "label" => "Aluno", "type" => "string", "html" => "<div style=\"text-align: left;\">{atenomedoaluno}</div>" ) );
                    }
                    continue;
                break;
                case 'ano_exercicio':
                    array_push( $coluna, array( "campo" => "ano_exercicio", "label" => "Ano de Exercício", "type" => "string" ) );
                    continue;
                break;
            }
	}
//	ver($coluna, d);
	return $coluna;
}

?>