<?php
/**
 * Classes de mapeamento para o acesso ao serviço WSReceita.
 * @version $Id: WSReceitaMap.php 87061 2014-09-19 13:25:44Z maykelbraz $
 */

/**
 * Classe de definição de mapeamentos do webservice.
 * @see WSReceita
 */
class WSReceitaMap
{
    public static function getClassmap()
    {
        return self::$classmap;
    }

    private static $classmap = array(
        'consultarDetalhesPorGrupo'=>'ConsultarDetalhesPorGrupo',
        'credencialDTO'=>'CredencialDTO',
        'baseDTO'=>'BaseDTO',
        'consultarDetalhesPorGrupoResponse'=>'ConsultarDetalhesPorGrupoResponse',
        'retornoCaptacaoDetalheBaseExternaDTO'=>'RetornoCaptacaoDetalheBaseExternaDTO',
        'detalhesCaptacaoBaseExterna'=>'DetalhesCaptacaoBaseExterna',
        'retornoDTO'=>'RetornoDTO',
        'captacaoDetalheBaseExternaDTO'=>'CaptacaoDetalheBaseExternaDTO',
        'valoresBaseExterna'=>'ValoresBaseExterna',
        'captacaoValorBaseExternaDTO'=>'CaptacaoValorBaseExternaDTO',
        'captarBaseExterna'=>'CaptarBaseExterna',
        'captacaoBaseExternaDTO'=>'CaptacaoBaseExternaDTO',
        'detalhesBaseExterna'=>'DetalhesBaseExterna',
        'disponibilidades'=>'Disponibilidades',
        'disponibilidadeCaptacaoBaseExternaDTO'=>'DisponibilidadeCaptacaoBaseExternaDTO',
        'captarBaseExternaResponse'=>'CaptarBaseExternaResponse',
        'retornoCaptacaoBaseExternaDTO'=>'RetornoCaptacaoBaseExternaDTO',
        'captacoesBaseExterna'=>'CaptacoesBaseExterna',
        'consultarDisponibilidadeCaptacaoBaseExterna'=>'ConsultarDisponibilidadeCaptacaoBaseExterna',
        'consultarDisponibilidadeCaptacaoBaseExternaResponse'=>'ConsultarDisponibilidadeCaptacaoBaseExternaResponse',
    );
}

class ConsultarDetalhesPorGrupo
{
    public $credencial; // -- CredencialDTO
    public $codigoCaptacaoBaseExterna; // -- int
    public $grupoNaturezaReceita; // -- string
}

class CredencialDTO
{
    public $perfil; // -- int
    public $senha; // -- string
    public $usuario; // -- string
}

class BaseDTO
{
}

class ConsultarDetalhesPorGrupoResponse
{
    public $return; // -- RetornoCaptacaoDetalheBaseExternaDTO
}

class RetornoCaptacaoDetalheBaseExternaDTO
{
    public $detalhesCaptacaoBaseExterna; // -- DetalhesCaptacaoBaseExterna
}

class DetalhesCaptacaoBaseExterna
{
    public $detalheCaptacaoBaseExterna; // -- CaptacaoDetalheBaseExternaDTO
}

class RetornoDTO
{
    public $mensagensErro; // -- string
    public $sucesso; // -- boolean
}

class CaptacaoDetalheBaseExternaDTO
{
    public $codigoNaturezaReceita; // -- string
    public $codigoUnidadeRecolhedora; // -- string
    public $subNatureza; // -- string
    public $justificativa; // -- string
    public $metodologia; // -- string
    public $memoriaDeCalculo; // -- string
    public $valoresBaseExterna; // -- ValoresBaseExterna
    public $versao; // -- int
    public $usuarioInclusao; // -- string
    public $usuarioAlteracao; // -- string
}

class ValoresBaseExterna
{
    public $valorBaseExterna; // -- CaptacaoValorBaseExternaDTO
}

class CaptacaoValorBaseExternaDTO
{
    public $exercicio; // -- int
    public $valor; // -- double
    public $usuarioInclusao; // -- string
    public $usuarioAlteracao; // -- string
}

class CaptarBaseExterna
{
    public $credencial; // -- CredencialDTO
    public $captacaoBaseExterna; // -- CaptacaoBaseExternaDTO
}

class CaptacaoBaseExternaDTO
{
    public $codigoCaptacaoBaseExterna; // -- int
    public $descricao; // -- string
    public $detalhesBaseExterna; // -- DetalhesBaseExterna
    public $disponibilidades; // -- Disponibilidades
}

class DetalhesBaseExterna
{
    public $detalheBaseExterna; // -- CaptacaoDetalheBaseExternaDTO
}

class Disponibilidades
{
    public $disponibilidade; // -- DisponibilidadeCaptacaoBaseExternaDTO
}

class DisponibilidadeCaptacaoBaseExternaDTO
{
    public $grupoNaturezaReceita; // -- string
    public $exercicio; // -- int
    public $abertoParaCaptacacao; // -- boolean
    public $inicioDisponibilidade; // -- dateTime
    public $fimDisponibilidade; // -- dateTime
}

class CaptarBaseExternaResponse
{
    public $return; // -- RetornoCaptacaoBaseExternaDTO
}

class RetornoCaptacaoBaseExternaDTO
{
    public $captacoesBaseExterna; // -- CaptacoesBaseExterna
}

class CaptacoesBaseExterna
{
    public $captacaoBaseExterna; // -- CaptacaoBaseExternaDTO
}

class ConsultarDisponibilidadeCaptacaoBaseExterna
{
    public $credencial; // -- CredencialDTO
}

class ConsultarDisponibilidadeCaptacaoBaseExternaResponse
{
    public $return; // -- RetornoCaptacaoBaseExternaDTO
}
