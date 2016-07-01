<?php

header('content-type: text/plain');




global $nome_bd;
       $nome_bd     = 'simec_desenvolvimento';

global $servidor_bd;
       $servidor_bd = 'simec-d';

global $porta_bd;
       $porta_bd    = '5432';

global $usuario_db;
       $usuario_db  = 'seguranca';

global $senha_bd;
       $senha_bd    = 'phpseguranca';


//$db = new cls_banco();


require_once "../adodb/adodb.inc.php";
require_once "../includes/ActiveRecord/ActiveRecord.php";
require_once "../includes/ActiveRecord/classes/AreaCurso.php";
require_once "../includes/ActiveRecord/classes/CursoTecnico.php";



$csv = file('../xxx.txt');

foreach ($csv as $i => $linha) {
    if ($i == 0)
        continue;

    $dados = explode("\t", $linha);

    if (sizeof($dados) != 8) {
        continue;
    }

    if (($aretitulo = trim($dados[0])) != '' &&
        ($aredsc    = trim($dados[1])) != '')
    {
        $area = new AreaCurso();
        $area->aretitulo = utf8_decode($aretitulo);
        $area->aredsc    = utf8_decode($aredsc);

        //$area->save();
    }

    $cte = new CursoTecnico();
    $cte->areid             = $area->getPrimaryKey();
    $cte->crstitulo         = utf8_decode(trim($dados[2]));
    $cte->crscargahoraria   = utf8_decode(trim(str_replace('horas', '', $dados[3])));
    $cte->crsdsc            = utf8_decode(trim($dados[4]));
    $cte->crstema           = utf8_decode(trim($dados[5]));
    $cte->crsatuacao        = utf8_decode(trim($dados[6]));
    $cte->crsinfraestrutura = utf8_decode(trim($dados[7]));

    //$cte->save();

    /*@!
     * @ AreaCurso
     * Eixo
     * Descritivo do Eixo
     *
     * @ CursoTecnico
     * Curso
     * Carga Horária Mínima
     * Descritivo do Curso
     * Possibilidades de temas a serem abordados na formação
     * Possibilidades de atuação
     * Infra-estrutura recomendada
     */
}





