<?php
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';


$co_funcionalidade      = (int) $_POST['co_funcionalidade'];
$co_inventario          = (int) $_POST['co_inventario'];
$co_agrupador           = (int) $_POST['co_agrupador'];
$co_tipo_funcionalidade = (int) $_POST['co_tipo_funcionalidade'];
$sidid                  = (int) $_POST['sidid'];
$qt_td                  = (float) $_POST['qtdtd'];
$qt_alr_rlr             = (float) $_POST['qtdalrrlr'];
$no_agrupador           = utf8_decode($_POST['no_agrupador']);
$ds_funcionalidade      = utf8_decode($_POST['ds_funcionalidade']);
$ds_alr_rlr             = utf8_decode($_POST['ds_alr_rlr']);
$ds_td                  = $_POST['ds_td'];
$tp_complexidade        = $_POST['tp_complexidade'];
$qtd_pf                 = $_POST['qtd_pf'];

//gera um novo inventário caso não exista
if( empty($co_inventario) )
{
    $inventarioRepositorio  = new InventarioRepositorio();
    
    if($inventarioRepositorio->possuiInventarioAtivo( $sidid )) {
        echo json_encode( array( 'status' => false,'msg' => utf8_encode('Não é possível gerar um inventário de um sistema já inventariado')
            ) 
        );
        
        exit;
    }
    
    
    $dados  = array(
        'sidid'         => $sidid,
        'dt_cadastro'   => "'". date('Y/m/d') ."'",
        'usucpf'        => "'". $_SESSION['usucpf'] ."'",
        'st_inventario' => "'A'"
    );
    
    $co_inventario = $inventarioRepositorio->cadastrar($dados);
    
    if($co_inventario == false) {
        echo json_encode( array( 'status' => false,'msg' => utf8_encode('Não é possível gerar um novo inventário')
            ) 
        );
        exit;
    }
    
    
}

//gera um novo agrupador caso não exista
if( empty( $co_agrupador ) && !empty( $no_agrupador ) )
{
    $agrupadorFuncionalidade    = new AgrupadorFuncionalidade();
    $agrupadorFuncionalidade->no_agrupador  = $no_agrupador;
    $agrupadorFuncionalidade->dt_cadastro   = date('Y/m/d');
    $agrupadorFuncionalidade->st_agrupador_funcionalidade = 'A';
    
    $co_agrupador_existente = $agrupadorFuncionalidade->verificaSeExisteAgrupadorPorNome($no_agrupador);
    
    if ($co_agrupador_existente == false){
    	$co_agrupador = $agrupadorFuncionalidade->salvarNovo( $no_agrupador, $sidid, date('Y/m/d') );
   } 
   else {
   	$co_agrupador = $co_agrupador_existente;
   	
   }
    
    //$co_agrupador = $agrupadorFuncionalidade->salvar();
    
    if($co_agrupador == false) {
        echo json_encode( array( 'status' => false,'msg' => utf8_encode('Não é possível gerar um novo agrupador de funcionalidade')
            ) 
        );
        exit;
    }
    
}

$funcionalidade = new Funcionalidade();
$funcionalidade->co_inventario          = $co_inventario;
$funcionalidade->co_agrupador           = $co_agrupador;
$funcionalidade->co_tipo_funcionalidade = $co_tipo_funcionalidade;
$funcionalidade->ds_funcionalidade      = $ds_funcionalidade;
$funcionalidade->ds_alr_rlr             = $ds_alr_rlr;
$funcionalidade->ds_td                  = $ds_td;
$funcionalidade->qt_alr_rlr             = $qt_alr_rlr;
$funcionalidade->qt_td                  = $qt_td;
$funcionalidade->tp_complexidade        = $tp_complexidade;
$funcionalidade->qtd_pf                 = $qtd_pf;

if( $funcionalidade->verificaSeExisteFuncionalidade( $ds_funcionalidade, $co_agrupador, $no_agrupador, $co_funcionalidade ))
    {
        echo json_encode( array( 'status' => false,'msg' => utf8_encode('Funcionalidade já cadastrada para esse sistema e agrupador')
            ) 
        );
        exit;
    }


if( !empty( $co_funcionalidade ) ) 
{
    $funcionalidade->co_funcionalidade          = $co_funcionalidade;
    
    if( !$funcionalidade->salvar() ) {
        echo json_encode( array( 'status' => false,'msg' => utf8_encode('Não foi possível atualizar a funcionalidade')
            ) 
        );
        exit;
    }
    
}
else 
{
    $co_funcionalidade   = $funcionalidade->salvar();
    
    if($co_funcionalidade == false) {
        echo json_encode( array( 'status' => false,'msg' => utf8_encode('Não foi possível gerar a funcionalidade')
            ) 
        );
        exit;
    }
}

$funcionalidade->commit();

echo json_encode( array( 'status' => true,'dados' => array(
    'co_funcionalidade' => $co_funcionalidade,
    'co_inventario'     => $co_inventario,
    'co_agrupador'      => $co_agrupador
    ) )
);


