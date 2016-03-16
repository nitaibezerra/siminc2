<?php
    header('Content-Type: text/html; charset=iso-8859-1'); 
    
    ini_set("memory_limit","500M");
	set_time_limit(0);
    
    
	if ($_REQUEST['filtrosession']){
		$filtroSession = $_REQUEST['filtrosession'];
	}
?>

<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
<?php
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	$sql   = monta_sql();
	$dados = $db->carregar($sql);
	$agrup = monta_agp();
	$col   = monta_coluna();
	
	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $dados); 
	$r->setColuna($col);
	$r->setBrasao(true);
	$r->setTotNivel(true);
	$r->setEspandir(false);
	$r->setMonstrarTolizadorNivel(true);
	$r->setTotalizador(true);
	$r->setTolizadorLinha(false);

	if($_POST['tipo_relatorio'] == 'xls'){
		ob_clean();
		$nomeDoArquivoXls="relatorio_avaliacao_final_".date('d-m-Y_H_i');
		echo $r->getRelatorioXls();
	}elseif($_POST['tipo_relatorio'] == 'visual'){
		echo $r->getRelatorio();	
	}
?>

</body>
</html>

<?php 
function monta_sql(){
	global $filtroSession;
	extract($_POST);
	
	if( $resavaliacaopendente != ''){
		$where[0] = " AND resavaliacaopendente = $resavaliacaopendente ";		
	}
	
	$sql = "    
        SELECT	sersiape,
                sernome,
                replace(to_char(cast(sercpf as bigint), '000:000:000-00'), ':', '.') as sercpf,
                soma_nota_auto, 
                soma_nota_sup, 
                soma_nota_eqp,
                avaliacao_final,
                CASE
                    WHEN avaliacao_final = 0 THEN '0'
                    WHEN avaliacao_final > 0 AND avaliacao_final <= 30 THEN '6'
                    WHEN avaliacao_final >= 31 AND avaliacao_final <= 40 THEN '8'
                    WHEN avaliacao_final >= 41 AND avaliacao_final <= 50 THEN '10'
                    WHEN avaliacao_final >= 51 AND avaliacao_final <= 60 THEN '12'
                    WHEN avaliacao_final >= 61 AND avaliacao_final <= 70 THEN '14'
                    WHEN avaliacao_final >= 71 AND avaliacao_final <= 80 THEN '16'
                    WHEN avaliacao_final >= 81 AND avaliacao_final <= 90 THEN '18'
                    WHEN avaliacao_final >= 91 AND avaliacao_final <= 100 THEN '20'
                END AS pontos,
                falta_nota,
                resavaliacaopendente
        FROM (

            SELECT  s.sersiape,
                    s.sernome, 
                    s.sercpf,
                    p.soma_nota_auto, 
                    sp.soma_nota_sup,
                    e.soma_nota_eqp,

                    CASE WHEN ( sp.soma_nota_sup IS NOT NULL AND e.soma_nota_eqp IS NOT NULL ) 
                            THEN ROUND( ( p.soma_nota_auto * 0.15 ) + ( sp.soma_nota_sup * 0.60) + ( e.soma_nota_eqp * 0.25) )

                         WHEN ( sp.soma_nota_sup IS NOT NULL ) 
                            THEN ROUND( ( p.soma_nota_auto * 0.275) + ( sp.soma_nota_sup * 0.725 ) )
                         ELSE 0
                    END AS avaliacao_final,

                    CASE WHEN p.soma_nota_auto IS NULL THEN 'NÃO A AUTO AVALIAÇÃO, MEDIA NÃO CALCULADA.'
                         WHEN sp.soma_nota_sup IS NULL THEN 'NÃO A AVALIAÇÃO DO SUPERVISOR, MEDIA NÃO CALCULADA.'
                         WHEN ( e.soma_nota_eqp IS NULL AND s.sercomequipe = TRUE ) THEN 'NÃO A AVALIAÇÃO DE EQUIPE, MEDIA NÃO CALCULADA.'
                         ELSE 'MEDIA CALCULADA'		
                    END AS falta_nota,		

                    CASE WHEN resavaliacaopendente = 'f'
                        THEN 'FINALIZADA'
                        ELSE 'NÃO FINALIZADA'
                    END AS resavaliacaopendente

            FROM gestaopessoa.respostaavaliacao r
            JOIN gestaopessoa.servidor s ON s.sercpf = r.sercpf

            LEFT JOIN(
                SELECT  sercpf,
                        SUM( ROUND(nota) ) as soma_nota_auto 
                FROM(
                    SELECT  s.sercpf,  
                            AVG( (d.defpeso * r.resnota) ) AS nota
                    FROM gestaopessoa.servidor s
                    INNER JOIN gestaopessoa.respostaavaliacao r ON r.sercpf = s.sercpf
                    INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
                    WHERE seranoreferencia = {$_SESSION['exercicio']} AND r.resano = {$_SESSION['exercicio']} AND tavid = 1
                    GROUP BY s.sercpf, s.sernome, d.defid
                ) AS f   
                GROUP BY sercpf
            ) AS p ON p.sercpf = s.sercpf


            LEFT JOIN(
                SELECT  sercpf,
                        SUM( ROUND(nota) ) as soma_nota_sup 
                FROM(
                    SELECT  s.sercpf,  
                            AVG( (d.defpeso * r.resnota) ) AS nota
                    FROM gestaopessoa.servidor s
                    INNER JOIN gestaopessoa.respostaavaliacao r ON r.sercpf = s.sercpf
                    INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
                    WHERE seranoreferencia = {$_SESSION['exercicio']} AND r.resano = {$_SESSION['exercicio']} AND tavid = 2
                    GROUP BY s.sercpf, s.sernome, d.defid
                ) AS f   
                GROUP BY sercpf
            ) AS sp ON sp.sercpf = s.sercpf


            LEFT JOIN(
                SELECT  sercpf,
                        SUM( ROUND(nota) ) as soma_nota_eqp 
                FROM(
                    SELECT  s.sercpf,  
                            AVG( (d.defpeso * r.resnota) ) AS nota
                    FROM gestaopessoa.servidor s
                    INNER JOIN gestaopessoa.respostaavaliacao r ON r.sercpf = s.sercpf
                    INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
                    WHERE seranoreferencia = {$_SESSION['exercicio']} AND r.resano = {$_SESSION['exercicio']} AND tavid = 3
                    GROUP BY s.sercpf, s.sernome, d.defid
                ) AS f   
                GROUP BY sercpf
            ) AS e ON e.sercpf = s.sercpf

            WHERE seranoreferencia = {$_SESSION['exercicio']}  AND r.resano = {$_SESSION['exercicio']}

            GROUP BY s.sersiape, s.sercpf, s.sernome, resavaliacaopendente, p.soma_nota_auto, sp.soma_nota_sup, soma_nota_eqp, s.sercomequipe
        ) AS AVALIACAO
        
        --FILTRO
	--Status
	".$where[0]."

        ORDER BY resavaliacaopendente, sernome    
    ";
    return $sql;
}

function monta_coluna(){
	
	$colunas = $_POST['colunas'];
	$colunas = $colunas ? $colunas : array();
	
	$coluna = array();
	
	foreach ($colunas as $val){
		switch ($val) {
			case 'sersiape':
				array_push( $coluna,
                        array(	"campo"		=> "sersiape",
                                "label" 	=> "SIAPE",
                                "type"	  	=> "string"
                        )
				);
				continue;
			break;
			case 'sercpf':
				array_push( $coluna, 
                        array(	"campo" 	=> "sercpf",
                                "label"		=> "CPF",
                                "type"	  	=> "string"
                        ) 
				);
				continue;
			break;
			case 'sernome':
                if($_POST['tipo_relatorio'] == 'xls'){
                    array_push( $coluna, 
                            array(	"campo" 	=> "sernome",
                                    "label"		=> "Servidor",
                                    "type"	  	=> "string"
                            ) 
                    );
                }else{
                    array_push( $coluna, 
                            array(	"campo" 	=> "sernome",
                                    "label"		=> "Servidor",
                                    "type"	  	=> "string",
                                    "html"	  	=> "<div style=\"text-align: left;\">{sernome}</div>"
                            ) 
                    );
                }
				continue;
			break;
			case 'soma_nota_auto':
				array_push( $coluna, 
                        array(	"campo" 	=> "soma_nota_auto",
                                "label"		=> "Nota Auto-Avaliação",
                                "type"	  	=> "numeric"
                        )
				);
				continue;
			break;
			case 'soma_nota_sup':
				array_push( $coluna, 
                        array(	"campo" 	=> "soma_nota_sup",
                                "label"		=> "Nota Supervisor",
                                "type"	  	=> "numeric"
                        ) 
				);
				continue;
			break;
			case 'soma_nota_eqp':
				array_push( $coluna, 
                        array(	"campo" 	=> "soma_nota_eqp",
                                "label"		=> "Nota Equipe",
                                "type"	  	=> "numeric"
                        ) 
				);
				continue;
			break;			
			case 'avaliacao_final':
				array_push( $coluna, 
                        array(	"campo" 	=> "avaliacao_final",
                                "label"		=> "Avaliação Final",
                                "type"	  	=> "numeric"
                        ) 
				);
				continue;
			break;
			case 'pontos':
				array_push( $coluna, 
                        array(	"campo" 	=> "pontos",
                                "label"		=> "Pontuação",
                                "type"	  	=> "numeric"
                        ) 
				);
				continue;
			break;
			case 'falta_nota':
                if($_POST['tipo_relatorio'] == 'xls'){
                    array_push( $coluna, 
                            array(	"campo" 	=> "falta_nota",
                                    "label"		=> "Status",
                                    "type"	  	=> "string"
                            ) 
                    );
                }else{
                    array_push( $coluna, 
                            array(	"campo" 	=> "falta_nota",
                                    "label"		=> "Status",
                                    "type"	  	=> "string",
                                    "html"	  	=> "<div style=\"text-align: left;\">{falta_nota}</div>"
                            ) 
                    );
                }
				continue;
			break;
			case 'resavaliacaopendente':
                if($_POST['tipo_relatorio'] == 'xls'){
                    array_push( $coluna, 
                            array(	"campo" 	=> "resavaliacaopendente",
                                    "label"		=> "Situação",
                                    "type"	  	=> "string"
                            ) 
                    );
                }else{
                    array_push( $coluna, 
                            array(	"campo" 	=> "resavaliacaopendente",
                                    "label"		=> "Situação",
                                    "type"	  	=> "string",
                                    "html"	  	=> "<div style=\"text-align: left;\">{resavaliacaopendente}</div>"
                            ) 
                    );
                }
				continue;
			break;
		}
	}	
	//ver($coluna, d);	
	return $coluna;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	
	$agp = 	array(	"agrupador" => array(), 
					"agrupadoColuna" => array("sersiape","sercpf","sernome","soma_nota_auto","soma_nota_sup","soma_nota_eqp","avaliacao_final","pontos","falta_nota","resavaliacaopendente")
			);
			
	$count = 1;
	$i = 0;
/*
	foreach ($agrupador as $val){
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}	
		
		switch ($val) {
			case 'estuf':
				array_push($agp['agrupador'], array("campo" => "estuf", "label" => "$var Estado") );				
		   		continue;
		    break;
		    case 'mundescricao':
				array_push($agp['agrupador'], array("campo" => "mundescricao", "label" => "$var Município") );					
		    	continue;
		    break;		    	
		    case 'pdiesfera':
				array_push($agp['agrupador'], array("campo" => "pdiesfera", "label" => "$var Esfera"));					
		    	continue;			
		    break;
		    case 'pdicodinep':
				array_push($agp['agrupador'], array("campo" => "pdicodinep","label" => "$var Código INEP"));					
		   		continue;			
		    break;
		    case 'pdenome':
				array_push($agp['agrupador'], array("campo" => "pdenome","label" => "$var Escola"));					
		   		continue;			
		    break;		    
		}
		$count++;
	}
 * */
 
	#Agrupador padrão. 
	array_push($agp['agrupador'], array("campo" => "sercpf","label" => "CPF."));
		
	return $agp;
}
?>